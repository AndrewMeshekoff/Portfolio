#ifndef OBJECT_H
#define OBJECT_H
#include <glutil/glutil.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Texture.h"
#include "HillGenerator.h"
/*
struct Asset
{
	Scene* owner;

public:
	Asset();
	Asset(Scene* owner);

	virtual void Draw(glutil::MatrixStack modelMatrix);
};*/

class HillPlane
{
	Mesh* model;
	Texture* grassTexture;
	Texture* sandTexture;
	HillGenerator HeightMap;

	glm::vec3 position;
	glm::vec3 rotationAxis;
	float rotationDegrees;
	glm::vec3 scaling;

public:
	HillPlane(float w, float l, float h, unsigned int seed);

	void Draw(glutil::MatrixStack modelMatrix);

	~HillPlane();
};

class Skybox
{
	Mesh* model;
	Texture* cubeMap;

	glm::vec3 vertices[8] = {
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, 1.0),
		glm::vec3(-1.0, 1.0, 1.0),
		glm::vec3(-1.0, 1.0, -1.0),
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(-1.0, -1.0, 1.0)
	};

	unsigned int elements[36] = {
		0, 3, 2,
		0, 2, 1,
		4, 7, 3,
		4, 3, 0,
		5, 6, 7,
		5, 7, 4,
		1, 2, 6,
		1, 6, 5,
		0, 1, 5,
		0, 5, 4,
		3, 7, 6,
		3, 6, 2
	};

	float rotationDegrees;

public:
	Skybox(float rotateOffset);

	void Draw(glutil::MatrixStack modelMatrix);

	~Skybox();
};
#endif