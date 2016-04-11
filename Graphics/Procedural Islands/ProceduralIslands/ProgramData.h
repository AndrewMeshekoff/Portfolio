#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H
#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Scene.h"
#include "Timer.h"

const char DATA_PATH[] = "./Data/";

class ProgramData
{
	Timer frameTimer;
	int screenX;
	int screenY;

	unsigned int currentShader;

	GLuint viewMatrixBuffer;

	void BindViewBuffer();
public:
	Scene* currentScene;
	std::vector<ShaderProgram*> programList;
	Timer programTime;

	ProgramData();
	ProgramData(int screenX, int screenY);
	void Setup();
	void SetCurrentScene(Scene* newScene);
	void SetShader(int index);
	const GLuint GetProgramHandle(int index);

	void UpdatePersBlock(const glm::mat4& pers);
	void UpdateModToCamBlock(const glm::mat4& transform);

	int ScreenWidth();
	int ScreenHeight();

	void SceneDisplay();
	void SceneReshape(int w, int h);
	void HandleKey(unsigned char key, int x, int y);
	void MouseMotion(int x, int y);
	void MouseButton(int button, int state, int x, int y);
	void MouseWheel(int wheel, int direction, int x, int y);

	~ProgramData();
};



#endif