#include "ObjectPole.h"

ObjectPole::ObjectPole(glm::vec2 initRotation, float thetaScaling, float phiScaling)
	:thetaScaling(thetaScaling), phiScaling(phiScaling)
{
	currRotation.x = fmod(initRotation.x, 360.0);
	currRotation.y = fmod(initRotation.y, 360.0);
}

glm::vec2 ObjectPole::GetTotalRotation()
{
	glm::ivec2 deltaPos = currPos - initDragPos;
	glm::vec2 deltaRot = glm::vec2(deltaPos.y * thetaScaling, deltaPos.x * phiScaling);

	glm::vec2 totalRot = currRotation;
	totalRot += deltaRot;
	totalRot.x = fmod(totalRot.x, 360.0);
	totalRot.y = fmod(totalRot.y, 360.0);

	return totalRot;
}

void ObjectPole::MouseMotion(int x, int y)
{
	currPos.x = x;
	currPos.y = y;
}

void ObjectPole::MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			isDraging = true;
			initDragPos = glm::ivec2(x, y);
		}
		else if (state == GLUT_UP)
		{
			isDraging = false;
			currRotation = GetTotalRotation();
		}
	}
}

glm::vec2 ObjectPole::getRotation()
{
	if (isDraging)
		return GetTotalRotation();
	else
		return currRotation;
}