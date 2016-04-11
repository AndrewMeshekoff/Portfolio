#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include <glutil/glutil.h>
#include <glm/glm.hpp>
#include <time.h>
#include "Scene.h"
#include "ProgramData.h"
#include "FrameBuffer.h"

extern ProgramData ProgramContext;

Scene::Scene()
	:mainCamera(glm::vec3(0.0, 5.0, 10.0),
				glm::vec3(50.0, 20.0, 0.0),
				glm::vec3(0.0, 1.0, 0.0),
				1.0),
	cameraRotation(glm::vec2(20.0, 0.0), 0.5, -0.3)
{
	islandAsset = NULL;
	skyAsset = NULL;
	seaAsset = NULL;
}

void Scene::GenerateHillPlane()
{
	time_t ticks;
	time(&ticks);
	islandAsset = new HillPlane(100.0f, 18.0f, 100.0f, (unsigned int)ticks);
}

void Scene::GenerateObjects()
{
	GenerateHillPlane();
	skyAsset = new Skybox(0.0);
	seaAsset = new WaterPlane(100.0, 0.0);
}

void Scene::SetupScene()
{
	fzNear = 1.0f;
	fzFar = 1000.0f;

	lightDirection = glm::normalize(glm::vec3(1.0, 1.0, -1.0));

	GenerateObjects();
}

void Scene::RenderSceneReflection(float height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutil::MatrixStack modelToCameraMatrix;
	modelToCameraMatrix.SetMatrix(mainCamera.GetCameraMatrix());

	modelToCameraMatrix.Push();
	modelToCameraMatrix.Scale(1.0, -1.0, 1.0);
	skyAsset->Draw(modelToCameraMatrix);

	//Update Light
	ProgramContext.SetShader(2);
	glm::vec3 cameraSpaceLight(glm::mat3(modelToCameraMatrix.Top()) * lightDirection);
	glUniform3fv(ProgramContext.programList[2]->uniforms[UniformEnums::LIGHT_DIR], 1, glm::value_ptr(cameraSpaceLight));
	modelToCameraMatrix.Pop();

	islandAsset->DrawReflection(modelToCameraMatrix, height);
}

void Scene::Display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutil::MatrixStack modelToCameraMatrix;
	modelToCameraMatrix.SetMatrix(mainCamera.GetCameraMatrix());
	skyAsset->Draw(modelToCameraMatrix);

	//Update Light
	ProgramContext.SetShader(0);
	glm::vec3 cameraSpaceLight(glm::mat3(modelToCameraMatrix.Top()) * lightDirection);
	glUniform3fv(ProgramContext.programList[0]->uniforms[UniformEnums::LIGHT_DIR], 1, glm::value_ptr(cameraSpaceLight));

	islandAsset->Draw(modelToCameraMatrix);
	seaAsset->Draw(modelToCameraMatrix);

	glutPostRedisplay();
	glutSwapBuffers();
}


void Scene::Reshape(int w, int h)
{
	float fov = 78.0;
	float fovScaling = glm::tan((fov * 3.14159 / 180) / 2.0f);

	glm::mat4 persMatrix(0.0f);
	persMatrix[0][0] = (h / (float)w) / fovScaling;
	persMatrix[1][1] = 1 / fovScaling;
	persMatrix[2][2] = (fzNear + fzFar) / (fzNear - fzFar);
	persMatrix[2][3] = -1.0;
	persMatrix[3][2] = 2 * fzFar * fzNear / (fzNear - fzFar);

	cameraToClipMatrix = persMatrix;
	ProgramContext.UpdatePersBlock(persMatrix);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	seaAsset->Reshape(w, h);
	glutPostRedisplay();
}

void Scene::keyboard(unsigned char key, int x, int y)
{
	glm::vec3 lookDir;
	glm::vec3 rightDir;

	glm::vec3 campos = mainCamera.GetEyePosition();
	switch (key)
	{
	case 'a':
		lookDir = mainCamera.GetLookDirection();
		lookDir.y = 0.0;
		lookDir = glm::normalize(lookDir);
		rightDir = glm::cross(lookDir, glm::vec3(0.0, 1.0, 0.0));
		mainCamera.OffsetAnchor(-rightDir);
		break;
	case 'd':
		lookDir = mainCamera.GetLookDirection();
		lookDir.y = 0.0;
		lookDir = glm::normalize(lookDir);
		rightDir = glm::cross(lookDir, glm::vec3(0.0, 1.0, 0.0));
		mainCamera.OffsetAnchor(rightDir);
		break;
	case 'w':
		lookDir = mainCamera.GetLookDirection();
		lookDir.y = 0.0;
		lookDir = glm::normalize(lookDir);
		mainCamera.OffsetAnchor(lookDir);
		break;
	case 's':
		lookDir = mainCamera.GetLookDirection();
		lookDir.y = 0.0;
		lookDir = glm::normalize(lookDir);
		mainCamera.OffsetAnchor(-lookDir);
		break;
	case 'i':
		mainCamera.OffsetSphere(glm::vec3(0.0, 5.0, 0.0));
		break;
	case 'k':
		mainCamera.OffsetSphere(glm::vec3(0.0, -5.0, 0.0));
		break;
	case 'j':
		mainCamera.OffsetSphere(glm::vec3(0.0, 0.0, 5.0));
		break;
	case 'l':
		mainCamera.OffsetSphere(glm::vec3(0.0, 0.0, -5.0));
		break;
	case ' ':
		delete islandAsset;
		GenerateHillPlane();
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void Scene::MouseMotion(int x, int y)
{
	cameraRotation.MouseMotion(x, y);
	mainCamera.SetSphereDirection(cameraRotation.getRotation());
}

void Scene::MouseButton(int button, int state, int x, int y)
{ 
	cameraRotation.MouseButton(button, state, x, y);
}

void Scene::MouseWheel(int wheel, int direction, int x, int y)
{
	mainCamera.OffsetRadius(direction * -5.0);
}

Scene::~Scene()
{
	delete skyAsset;
	delete islandAsset;
	delete seaAsset;
}