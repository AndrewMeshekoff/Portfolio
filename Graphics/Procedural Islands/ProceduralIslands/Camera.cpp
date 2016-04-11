#include "Camera.h"

Camera::Camera(glm::vec3 anchorPoint, glm::vec3 spherePosition, glm::vec3 upDirection, float minRadius)
	:anchorPoint(anchorPoint), spherePosition(spherePosition), upDirection(glm::normalize(upDirection)), minRadius(minRadius)
{}

glm::vec3 Camera::GetPosRelAnchor()
{
	float d2R = 3.14159 / 180;

	float r_xz = spherePosition.x * cosf(spherePosition.y * d2R);

	return glm::vec3(r_xz * sinf(spherePosition.z * d2R),
		spherePosition.x * sinf(spherePosition.y * d2R),
		r_xz * cosf(spherePosition.z * d2R));
}

glm::mat4 Camera::GetCameraMatrix()
{
	glm::vec3 posRelAnchor = GetPosRelAnchor();
	glm::vec3 globalPosition = anchorPoint + posRelAnchor;
	glm::vec3 lookDir = glm::normalize(-posRelAnchor);

	glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDirection));
	glm::vec3 orthUp = glm::cross(rightDir, lookDir);

	glm::mat4 rotation(1.0);
	rotation[0] = glm::vec4(rightDir, 0.0);
	rotation[1] = glm::vec4(orthUp, 0.0);
	rotation[2] = glm::vec4(-lookDir, 0.0);

	rotation = glm::transpose(rotation);

	glm::mat4 translation(1.0);
	translation[3] = glm::vec4(-globalPosition, 1.0);

	return rotation * translation;
}

glm::vec3 Camera::GetLookDirection()
{
	glm::vec3 posRelAnchor = GetPosRelAnchor();
	return glm::normalize(-posRelAnchor);
}

glm::vec3 Camera::GetEyePosition()
{
	return anchorPoint + GetPosRelAnchor();
}

void Camera::OffsetAnchor(glm::vec3 offset)
{
	anchorPoint += offset;
}

void Camera::OffsetSphere(glm::vec3 offset)
{
	spherePosition += offset;
}

void Camera::OffsetRadius(float radius)
{
	spherePosition.x += radius;

	if (spherePosition.x < minRadius)
		spherePosition.x = minRadius;
}

void Camera::SetSphereDirection(glm::vec2 direction)
{
	spherePosition.y = direction.x;
	spherePosition.z = direction.y;
}