// Author: Andrew Meshekoff
// Date: December, 2014
////////////////////////////////////////////////////////////

/*
controls: x - toggle shading mode
*/
#include <cmath>
#include <vector>
#include <fstream>
#include <GL/glut.h>
using namespace std;

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 900;

//Bounds for the viewspace
const float VIEW_LEFT = 0.0;
const float VIEW_RIGHT = 900;
const float VIEW_BOTTOM = 0.0;
const float VIEW_TOP = 900;
const float VIEW_NEAR = -400;
const float VIEW_FAR = 400;

//location of perspective point for viewspace
const int CAMERA_Z = -800;
const int CAMERA_X = (VIEW_RIGHT-VIEW_LEFT)/2.0;
const int CAMERA_Y = (VIEW_TOP-VIEW_BOTTOM)/2.0;

//location of light
const int LIGHT_X = 200;
const int LIGHT_Y = 800;
const int LIGHT_Z = -100;

// Retrieve random float
float randFloat()
{
	return rand() / static_cast<float>(RAND_MAX);
}
float randFloat(float min, float max)
{
	return randFloat() * (max - min) + min;
}

// A simple wrapper for store 3D vectors
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() : x(0.0), y(0.0), z(0.0)
	{}

	Vector3(float x, float y, float z)
    : x(x), y(y), z(z)
	{}

	Vector3(const Vector3 & v)
    : x(v.x), y(v.y), z(v.z)
	{}

	Vector3 operator+(const Vector3 & rhs) const
	{ return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator-(const Vector3 & rhs) const
	{ return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator-(const float & rhs) const
	{ return Vector3(x - rhs, y - rhs, z - rhs); }
	Vector3 operator*(float rhs) const
	{ return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator/(float rhs) const
	{ return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3 operator+=(const Vector3 & rhs)
	{ x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vector3 operator-=(const Vector3 & rhs)
	{ x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	Vector3 operator*=(float rhs)
	{ x *= rhs; y *= rhs; z *= rhs; return *this; }
	Vector3 operator/=(float rhs)
	{ x /= rhs; y /= rhs; z /= rhs; return *this; }

	float magnitude() const
	{ return sqrt(x * x + y * y + z * z); }
	void normalize()
	{ *this /= magnitude(); }
	Vector3 normalized() const
	{ return *this / magnitude(); }
	float dot(const Vector3 & rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	Vector3 cross(const Vector3 & rhs) const
	{
		return Vector3(y * rhs.z - z * rhs.y,
                       z * rhs.x - x * rhs.z,
                       x * rhs.y - y * rhs.x);
	}
};

//Point primitive
struct Point3D {
	int x, y, z;
	Vector3 dir;

	Point3D (): x(0), y(0), z(0), dir()
	{}
	Point3D (int x, int y, int z): x(x), y(y), z(z), dir()
	{}
};

//Triangle primitive
struct Triangle {
	int a, b, c, D;
	Vector3 N;
	double rc, gc, bc;
	Triangle (int a, int b, int c, Vector3 N, int D)
	: a(a), b(b), c(c), N(N), D(D)
	{}
};

// A simple wrapper to store colors
struct Color3d
{
	float r;
	float g;
	float b;
	float a;

	Color3d()
    : r(0.0), g(0.0), b(0.0), a(1.0)
	{}
	Color3d(float r, float g, float b, float a = 1.0)
    : r(r), g(g), b(b), a(a)
	{}

	Color3d operator * (const float & rhs) {
		return Color3d(r*rhs, g*rhs, b*rhs);
	}
	Color3d operator + (const Color3d & rhs) {
		return Color3d(this->r+rhs.r, this->g+rhs.g, this->b+rhs.b);
	}
};

//Ray structure
struct Ray
{
    Vector3 origin;
    Vector3 direction;
    Ray() {Vector3 d(0.0, 0.0, 1.0);  direction = d;}
    Ray(const Vector3& o, const Vector3& dir)
    {
        origin = o;
        Vector3 d(0.0, 0.0, 1.0);
        float mag = dir.magnitude();
        if (mag > 0.0) {d = dir;}
        direction = d;
    }
};

//Mode to switch between smooth and flat shading
bool s_mode = false;

//global primitives
vector<Point3D> points;
vector<Triangle> triangles;

//Light constants
const float k_diffuse = 0.6;
Color3d L_diffuse (1.0, 0.0, 0.0);
const float k_specular = 0.7;
Color3d L_specular (1.0, 2.0, 2.0);
const float k_amb = 0.4;
Color3d L_amb (0.7, 0.0, 0.0);

Vector3 getNorm(Point3D a, Point3D b, Point3D c) {
	Vector3 side_1(b.x-a.x, b.y-a.y, b.z-a.z);

	Vector3 side_2(c.x-a.x, c.y-a.y, c.z-a.z);

	return side_1.cross(side_2);
}

int solveD(Vector3 norm, Point3D a) {
	return -1*(norm.x*a.x+norm.y*a.y+norm.z*a.z);
}

//helper function to orient triangle normals
void orientNormal(Vector3 & norm, Point3D a, Point3D b, Point3D c) {
	Vector3 origin(CAMERA_X, CAMERA_Y, CAMERA_Z);
	Vector3 center((a.x+b.x+c.x)/3.0, (a.y+b.y+c.y)/3.0, (a.z+b.z+c.z)/3.0);
	Vector3 dir = origin - center;

	if(norm.dot(dir)<0)
		norm*=-1;
}


void renderPixel(int x, int y, Color3d& color, float sz = 1.0)
{
	glPointSize(sz);
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

float Det_3x3 (float * mat) {
	return mat[0]*(mat[4]*mat[8]-mat[5]*mat[7])
	-mat[1]*(mat[3]*mat[8]-mat[5]*mat[6])
	+mat[2]*(mat[3]*mat[7]-mat[4]*mat[6]);
}


//Returns parametric values if an intersection occurs
bool intersect (const Triangle & draw, Ray trace, float & t, float & B, float & g) {
	Point3D a = points[draw.a];
	Point3D b = points[draw.b];
	Point3D c = points[draw.c];

	float Ebx = b.x - a.x;
	float Eby = b.y - a.y;
	float Ebz = b.z - a.z;

	float Ecx = c.x - a.x;
	float Ecy = c.y - a.y;
	float Ecz = c.z - a.z;

	float o_ax = a.x - trace.origin.x;
	float o_ay = a.y - trace.origin.y;
	float o_az = a.z - trace.origin.z;

	float Tmat[9] = {-1*Ebx, -1*Ecx, o_ax,
					-1*Eby, -1*Ecy, o_ay,
					-1*Ebz, -1*Ecz, o_az};

	float Cmat[9] = {-1*Ebx, o_ax, trace.direction.x,
					-1*Eby, o_ay, trace.direction.y,
					-1*Ebz, o_az, trace.direction.z};

	float Bmat[9] = {o_ax, -1*Ecx, trace.direction.x,
					o_ay, -1*Ecy, trace.direction.y,
					o_az, -1*Ecz, trace.direction.z};

	float Amat[9] = {-1*Ebx, -1*Ecx, trace.direction.x,
					-1*Eby, -1*Ecy, trace.direction.y,
					-1*Ebz, -1*Ecz, trace.direction.z};

	float detA = Det_3x3(Amat);

	t = Det_3x3(Tmat)/detA;
	B = Det_3x3(Bmat)/detA;
	g = Det_3x3(Cmat)/detA;

	return t>0 && B>0 && g>0 && B+g<1;
}

//main raytracing algorithm
Color3d raytrace(Ray trace) {
	float t, t_temp, B, B_temp, g, g_temp;
	t = -1.0;
	int closeTri = -1;
	for(int i = 0; i<triangles.size(); i++) //loop: get closest interection triangle
		if(intersect(triangles[i], trace, t_temp, B_temp, g_temp))
			if(t_temp<t||t<0) {
				t = t_temp;
				B = B_temp;
				g = g_temp;
				closeTri = i;
			}

	if (closeTri>=0) { //if a triangle was found
		Vector3 pos = trace.origin + trace.direction*t;
		Vector3 l_pos(LIGHT_X, LIGHT_Y, LIGHT_Z);
		Vector3 light = (l_pos - pos).normalized();

		for(int i = 0; i<triangles.size(); i++) 		// loop through triangles for
			if(intersect(triangles[i], Ray(pos, light), // shadow ray
			t_temp, B_temp, g_temp)&&i!=closeTri)
				return L_amb*k_amb;

		Vector3 v = (trace.direction*-1).normalized();
		Vector3 h = (light+v)/(light+v).magnitude();
		Vector3 triNorm;
		if (s_mode) { //smooth shading normal
			triNorm = points[triangles[closeTri].a].dir*(2-B-g)
					+ points[triangles[closeTri].b].dir*(B+1-g)
					+ points[triangles[closeTri].c].dir*(g+1-B);
			triNorm.normalize();
		}
		else { //flat shading normal
			triNorm = triangles[closeTri].N.normalized();
		}
		return L_diffuse*k_diffuse*light.dot(triNorm) //phong equation
		+ L_specular*k_specular*pow(h.dot(triNorm), 40)
		+ L_amb*k_amb;
	}

	return Color3d(0.0, 0.0, 0.0);
}

void GLKeyboardPress (unsigned char key, int x, int y) {
	if (key=='x') //toggle modes
		s_mode = !s_mode;

	glutPostRedisplay();
}

void GLrender();

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Project 2 - amesh001");
	glMatrixMode(GL_PROJECTION_MATRIX);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
	glutDisplayFunc(GLrender);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glutKeyboardFunc(GLKeyboardPress);
}

void GLrender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	const float viewres_x = (VIEW_RIGHT-VIEW_LEFT)/WINDOW_WIDTH;
	const float viewres_y = (VIEW_TOP-VIEW_BOTTOM)/WINDOW_HEIGHT;


    for (int i = 0; i < WINDOW_HEIGHT; i++)
    {
        for (int j = 0; j < WINDOW_WIDTH; j++)
        {	 //point perspective
			Vector3 origin(j*viewres_x, i*viewres_y, VIEW_NEAR);
			Vector3 dir(origin.x-CAMERA_X, origin.y-CAMERA_Y, origin.z-CAMERA_Z);
			Ray viewray(origin, dir); // contructs perspective ray per pixel
			Color3d light = raytrace(viewray);
			renderPixel(j, i, light);
        }
    }

	glFlush();
	glutSwapBuffers();
}



//Creates Primitives from Input File
void createPrimitives(char * arg) {

	ifstream primFile(arg);
	if(!primFile.is_open())
		throw -1;

	int numPts, numTri;
	float x_tot = 0;
	float z_tot = 0;

	primFile >> numPts;
	primFile >> numTri;

	Point3D temp_pt; //get points
	for (int i=0; i<numPts; i++) {
		primFile >> temp_pt.x;
		primFile >> temp_pt.y;
		primFile >> temp_pt.z;
		points.push_back(temp_pt);
	}

	int p1, p2, p3; //get triangles
	for (int i=0; i<numTri; i++) {
		primFile >> p1;
		primFile >> p2;
		primFile >> p3;
		Vector3 norm = getNorm(points[p1], points[p2], points[p3]);
		orientNormal(norm, points[p1], points[p2], points[p3]); //corrects normals
		triangles.push_back(Triangle(p1, p2, p3, norm, solveD(norm, points[p1])));

		points[p1].dir+=norm; //stores interpolation normals on points
		points[p2].dir+=norm;
		points[p3].dir+=norm;
	}

	for (int i = 0; i<points.size(); i++)
		points[i].dir.normalize();

	primFile.close();
}

int main(int argc, char** argv)
{
	if(argc < 2)
		return 0;

	createPrimitives(argv[1]);
	GLInit(&argc, argv);
	glutMainLoop();

	return 0;
}
