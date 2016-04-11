#include <stdio.h>
#include <glload/gl_3_3.h>
#include <glload/gl_load.hpp>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "ProgramData.h"

#define WINDOW_X 1024
#define WINDOW_Y 768

ProgramData ProgramContext(WINDOW_X, WINDOW_Y);

void Display() { ProgramContext.SceneDisplay(); }

void Reshape(int w, int h) { ProgramContext.SceneReshape(w, h); }

void keyboard(unsigned char key, int x, int y) { ProgramContext.HandleKey(key, x, y); }

void MouseMotion(int x, int y) { ProgramContext.MouseMotion(x, y); }

void MouseButton(int button, int state, int x, int y) { ProgramContext.MouseButton(button, state, x, y); }

void MouseWheel(int wheel, int direction, int x, int y) { ProgramContext.MouseWheel(wheel, direction, x, y); }


void prompt()
{
	printf("Controls:\n"
		"\tW-Forward\n"
		"\tA-Left\n"
		"\tS-Backward\n"
		"\tD-Right\n"
		"\t[LMB + Drag]-Look\n"
		"\t[Scroll]-Zoom\n"
		"\t[Space]-Regenerate Map\n"
		"\tEsc-Quit\n");
}

int initContext(int argc, char**argv)
{
	glutInit(&argc, argv);

	unsigned int glutOptions = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

	glutInitDisplayMode(glutOptions);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitWindowSize(WINDOW_X, WINDOW_Y);
	glutInitWindowPosition(200, 200);

	int glutWindow = glutCreateWindow("Procedural Islands");

	glload::LoadTest loadResult;
	loadResult = glload::LoadFunctions();
	if (!loadResult)
	{
		fprintf(stderr, "error: gl_load could not load any functions");
		glutDestroyWindow(glutWindow);
		exit(0);
	}

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	ProgramContext.Setup();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutMouseWheelFunc(MouseWheel);
}



int main(int argc, char**argv)
{
	initContext(argc, argv);

	prompt();

	glutMainLoop();

	return 0;
}

