#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "ProgramData.h"
#include "ShaderProgram.h"
#include <vector>

ProgramData::ProgramData()
	:programList(), programTime(false, 0.0), frameTimer(false, 0.017)
{}

void ProgramData::Setup()
{
	IslandShader* defaultShader = new IslandShader();
	defaultShader->LoadShader(GL_VERTEX_SHADER, "PlanarLight.vs");
	defaultShader->LoadShader(GL_FRAGMENT_SHADER, "HeightTexture.fs");
	defaultShader->LinkProgram();
	programList.push_back(defaultShader);

	SkyboxShader* skyShader = new SkyboxShader();
	skyShader->LoadShader(GL_VERTEX_SHADER, "CubeMap.vs");
	skyShader->LoadShader(GL_FRAGMENT_SHADER, "CubeMap.fs");
	skyShader->LinkProgram();
	programList.push_back(skyShader);

	BindViewBuffer();

	SetShader(0);

	SetCurrentScene(new Scene());
	currentScene->SetupScene();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);
}

void ProgramData::BindViewBuffer()
{
	glUniformBlockBinding(GetProgramHandle(0), programList[0]->uniforms[UniformEnums::VIEW_BLOCK_INDEX], 0);
	glUniformBlockBinding(GetProgramHandle(1), programList[0]->uniforms[UniformEnums::VIEW_BLOCK_INDEX], 0);

	glGenBuffers(1, &viewMatrixBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, viewMatrixBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, viewMatrixBuffer, 0, sizeof(glm::mat4) * 2);
}

void ProgramData::SetCurrentScene(Scene* newScene)
{
	currentScene = newScene;
}

void ProgramData::SetShader(int index)
{
	GLuint handle = (programList[index])->GetProgramHandle();
	glUseProgram(handle);
}

const GLuint ProgramData::GetProgramHandle(int index)
{
	return (programList[index])->GetProgramHandle();
}

void ProgramData::UpdatePersBlock(const glm::mat4& matrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, viewMatrixBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
}

void ProgramData::UpdateModToCamBlock(const glm::mat4& matrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, viewMatrixBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(matrix));
}

void ProgramData::SceneDisplay()
{
	if (!frameTimer.IsPassed())
	{

		int t = frameTimer.TimeLeft() * 1000;
		Sleep((int)(t));

	}
	frameTimer.Reset();

	currentScene->Display();
}

void ProgramData::SceneReshape(int w, int h)
{
	currentScene->Reshape(w, h);
}

void ProgramData::HandleKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		return;
	default:
		currentScene->keyboard(key, x, y);
		break;
	}

	glutPostRedisplay();
}

/*ProgramData::~ProgramData()
{
	delete currentScene;
}*/
