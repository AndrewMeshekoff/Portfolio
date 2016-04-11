#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(std140) uniform viewMatrices
{
	uniform mat4 modelToCameraMatrix;
	uniform mat4 cameraToClipMatrix;
};

uniform mat3 worldToModelMatrix;
uniform mat4 texProjMatrix;

//Normal map optimization;
uniform vec3 eyeTanSpace;

smooth out vec3 modelPosition;
smooth out vec3 eyeDirection;
out vec4 clipTexProjCoord;

void main()
{
	eyeDirection = eyeTanSpace - position;

	modelPosition = position;
	vec4 clipPosition = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));
	gl_Position = clipPosition;
	clipTexProjCoord = texProjMatrix * clipPosition;
}