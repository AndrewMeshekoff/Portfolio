#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <glm/glm.hpp>
#include "Asset.h"
#include "Camera.h"
#include "ObjectPole.h"

class Scene
{

	HillPlane* islandAsset;
	Skybox* skyAsset;
	WaterPlane* seaAsset;
	ObjectPole cameraRotation;

	void UpdateObjects();
	void GenerateObjects();

	float fzNear;
	float fzFar;

	void GenerateHillPlane();
public:
	Camera mainCamera;
	glm::vec3 lightDirection;
	glm::mat4 cameraToClipMatrix;

	Scene();

	void SetupScene();
	void Display();
	void RenderSceneReflection(float height);
	void Reshape(int w, int h);
	void keyboard(unsigned char key, int x, int y);
	void MouseMotion(int x, int y);
	void MouseButton(int button, int state, int x, int y);
	void MouseWheel(int wheel, int direction, int x, int y);

	~Scene();
};










#endif
