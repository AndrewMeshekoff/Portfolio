#ifndef OBJECT_POLE_H
#define OBJECT_POLE_H
#include <glm/glm.hpp>
#include <GL/freeglut.h>



class ObjectPole
{
private:
	glm::ivec2 initDragPos;
	glm::ivec2 currPos;
	glm::vec2 currRotation;

	float thetaScaling;
	float phiScaling;

	bool isDraging;

	glm::vec2 GetTotalRotation();
public:
	ObjectPole(glm::vec2 initRotation, float thetaScaling, float phiScaling);

	void MouseMotion(int x, int y);
	void MouseButton(int button, int state, int x, int y);

	glm::vec2 getRotation();
};









#endif OBJECT_POLE_H
