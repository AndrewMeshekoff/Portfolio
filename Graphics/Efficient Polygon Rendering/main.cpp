// Name: Andrew Meshekoff
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <time.h>


/* Controls:
 * a-rotate left
 * d-rotate right
 * w-toggle wireframe
 *
 * Note:
 * Due to random nature of the color generation,
 * neigboring triangles may get very similar colors.
 * Just rerun the program if it is too hard to
 * discern between triangles.
 */

using namespace std;

//Data Structures-------------------------------------------------------------

//Point primitive
struct Point3D {
	int x, y, z;

	Point3D (): x(0), y(0), z(0)
	{}
	Point3D (int x, int y, int z): x(x), y(y), z(z)
	{}
};

//Triangle primitive
struct Triangle {
	Point3D a, b, c;
	double rc, gc, bc;
	Triangle (Point3D a, Point3D b, Point3D c) : a(a), b(b), c(c)
	{
		rc = (rand() % 101)/100.0;
		gc = ((rand()+5) % 101)/100.0;
		bc = ((rand()+11) % 101)/100.0;
	}
};

// A simple wrapper for to store 3D vectors
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
	{ return Vector3(x - rhs.x, y + rhs.y, z - rhs.z); }
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

//Global Constants and Variables----------------------------------------------

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

float * depthGrid = new float[WINDOW_WIDTH*WINDOW_HEIGHT]; //z-buffer

float x_rot = 0; //x-value to rotate about
float z_rot = 0; //y-value to rotate about

double angle = 0; // angle of rotation

bool w_mod = false; // wiremode variable

vector <Triangle> triangles;

//Helper Functions------------------------------------------------------------

int fact(int a) {
	int out = 1;

	for(; a>1; a--)
		out*=a;

	return out;
}

double choose (int n, int r) {
	if (n==r)
		return 1;
	return fact(n) / (fact(r)*fact(n-r));
}

void swapPts(Point3D & a, Point3D & b) {
	Point3D temp = a;
	a = b;
	b = temp;
}

Vector3 getPlaneEq(Point3D a, Point3D b, Point3D c) {
	Vector3 side_1(b.x-a.x, b.y-a.y, b.z-a.z);

	Vector3 side_2(c.x-a.x, c.y-a.y, c.z-a.z);

	return side_2.cross(side_1);
}

int abs(int a) { //absolute value
	if(a < 0)
		return -1*a;
	return a;
}

Point3D rotPoint (Point3D a) {
	Point3D b;
	b.x = cos(angle)*(a.x-x_rot)+sin(angle)*(a.z-z_rot)+x_rot;
	b.y = a.y;
	b.z = -1.0*sin(angle)*(a.x-x_rot)+cos(angle)*(a.z-z_rot)+z_rot;
	return b;
}

//Drawing Functions-----------------------------------------------------------

// Renders a quad at cell (x, y) with dimensions CELL_LENGTH
void renderPixel(int x, int y, float r = 1.0, float g = 1.0, float b = 1.0)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void renderLine(float ax, float ay, float bx, float by) {
	float m = (by-ay)/(bx-ax);
	float x, y;

	if(abs(m) <= 1) {
		int pixels = abs(bx-ax);
		x = ax;
		y = ay;
		int sign = (bx-ax)/abs(bx-ax);
		for(int i = 0; i<pixels; i++) {
				renderPixel(x, y);
				x = x + sign;
				y = y + m*sign;
		}
		renderPixel(x, y);
	}
	else {
		int pixels = abs(by-ay);
		x = ax;
		y = ay;
		int sign = (by-ay)/abs(by-ay);
		for(int i = 0; i<pixels; i++) {
				renderPixel(x, y);
				y = y + sign;
				x = x + 1/m*sign;
		}
		renderPixel(x, y);
	}
}

//draws the component triangles
void drawBasicTri(Point3D a, Point3D b, Point3D c) {

	float x_0, x_1, mi_l, mi_r, b_l, b_r, z_s, z;
	float y_min, y_max;
	if(c.y>a.y) { // for flat-side bottom triangle
		mi_l = static_cast<float>(c.x-a.x)/
		static_cast<float>(c.y-a.y);
		mi_r = static_cast<float>(c.x-b.x)/
		static_cast<float>(c.y-b.y);

		y_min = a.y;
		y_max = c.y;
		z_s = a.z;
	}
	else { // for flat-side top triangle
		mi_l = static_cast<float>(a.x-c.x)/
		static_cast<float>(a.y-c.y);
		mi_r = static_cast<float>(b.x-c.x)/
		static_cast<float>(b.y-c.y);

		y_min = c.y;
		y_max = a.y;
		z_s = c.z;
	}

	Vector3 plane = getPlaneEq(a, b, c);

	for(int y = y_min; y<=y_max; y++) {
		x_0=(y-c.y)*mi_l+c.x;
		x_1=(y-c.y)*mi_r+c.x;;
		if(x_0>x_1) {
			double temp = x_0;
			x_0 = x_1;
			x_1 = temp;
		}
		z = z_s;
		for(int x=x_0; x<=x_1; x++) {
			if((x<WINDOW_WIDTH&&x>0)&&(y<WINDOW_HEIGHT&&y>0)
			&&z<depthGrid[x+800*y]) { // check if closest
				renderPixel(x+0.5, y+0.5);
				depthGrid[x+800*y] = z;
			}
			if(plane.z!=0)
				z=z-plane.x/plane.z;
		}
		if(plane.z!=0)
			z_s=z_s-((plane.x*mi_l+plane.y)/plane.z);
	}
}

//splits the triangle horizontaly into component triangles
void drawTriangle(Point3D a, Point3D b, Point3D c) {
	if(a.y>b.y) //order bottom height to top height
		swapPts(a,b);
	if(a.y>c.y)
		swapPts(a,c);
	if(b.y>c.y)
		swapPts(b,c);

	if(c.y!=a.y) {
		double s = static_cast<float>(c.x-a.x)/(c.y-a.y);
		double c_0 = a.x-s*a.y;
		double s_1 = static_cast<float>(c.z-a.z)/(c.y-a.y);
		double c_1 = a.z-s_1*a.y;

		Point3D newP (s*b.y+c_0, b.y, s_1*b.y+c_1);

		if (newP.x<b.x) {
			if(b.y!=c.y)
				drawBasicTri(newP, b, c);
			if(b.y!=a.y)
				drawBasicTri(newP, b, a);
		}
		else {
			if(b.y!=c.y)
				drawBasicTri(b, newP, c);
			if(b.y!=a.y)
				drawBasicTri(b, newP, a);
		}
	}
}

//calls transformation helper function on all points
void applyTrans(const Triangle & tri) {
	drawTriangle(rotPoint(tri.a), rotPoint(tri.b), rotPoint(tri.c));
}

//wireframe function
void drawWireTri(const Triangle & tri) {
	Point3D a = rotPoint(tri.a);
	Point3D b = rotPoint(tri.b);
	Point3D c = rotPoint(tri.c);

	renderLine(a.x, a.y, b.x, b.y);
	renderLine(b.x, b.y, c.x, c.y);
	renderLine(c.x, c.y, a.x, a.y);
}

//glut Functions--------------------------------------------------------------

//Output function to OpenGL Buffer
void GL_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(w_mod)
		for(int i = 0; i<triangles.size(); i++)
			drawWireTri(triangles[i]);
    else {
		for(int i = 0; i<WINDOW_HEIGHT*WINDOW_WIDTH; i++) { //reset z-buffer
			depthGrid[i] = 10000; //maximum view distance = 10000;
		}
		for(int i = 0; i<triangles.size(); i++) {
			glColor3d(triangles[i].rc, triangles[i].gc, triangles[i].bc);
			applyTrans(triangles[i]);
		}
	}

    glutSwapBuffers();
}

//Take keyboard input
void GLKeyboardPress (unsigned char key, int x, int y) {
	if (key=='a') //rotate left
		angle += 0.2;
	else if (key=='d') //rotate right
		angle -= 0.2;
	else if (key=='w') { //toggle wireframe mode
		if(w_mod)
			w_mod = false;
		else
			w_mod = true;
	}

	if(angle > 6.28)
		angle -= 6.28;

	glutPostRedisplay();
}

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CS 130 - amesh001 - Project 1");
	glMatrixMode(GL_PROJECTION_MATRIX);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
	glutDisplayFunc(GL_render);
	glutKeyboardFunc(GLKeyboardPress);
}

//Creates Primitives from Input File
void createPrimitives(char * arg) {
	srand(time(NULL));

	ifstream primFile(arg);
	if(!primFile.is_open())
		throw -1;

	int numPts, numTri;
	float x_tot = 0;
	float z_tot = 0;

	primFile >> numPts;
	primFile >> numTri;

	Point3D pts[numPts]; //get points
	for (int i=0; i<numPts; i++) {
		primFile >> pts[i].x;
		primFile >> pts[i].y;
		primFile >> pts[i].z;

		x_tot += pts[i].x;
		z_tot += pts[i].z;
	}

	x_rot = x_tot/numPts;
	z_rot = z_tot/numPts;

	int p1, p2, p3; //get triangles
	for (int i=0; i<numTri; i++) {
		primFile >> p1;
		primFile >> p2;
		primFile >> p3;
		triangles.push_back(Triangle(pts[p1], pts[p2], pts[p3]));
	}

	primFile.close();
}

int main(int argc, char** argv)
{
	if(argc < 2)
		return 0;

	createPrimitives(argv[1]);
	GLInit(&argc, argv);
	glutMainLoop();

	delete[] depthGrid;
	return 0;
}


