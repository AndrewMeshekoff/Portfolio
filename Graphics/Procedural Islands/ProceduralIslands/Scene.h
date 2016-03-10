#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <glm/glm.hpp>
#include "Asset.h"
#include "Camera.h"

class Scene
{
	Camera mainCamera;

	HillPlane* islandAsset;
	Skybox* skyAsset;

	glm::vec3 lightDirection;

	void UpdateObjects();
	void GenerateObjects();

	float fzNear;
	float fzFar;

	void GenerateHillPlane();
public:
	glm::mat4 cameraToClipMatrix;

	Scene();

	void SetupScene();
	void Display();
	void Reshape(int w, int h);
	void keyboard(unsigned char key, int x, int y);

};










#endif
