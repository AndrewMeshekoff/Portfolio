#version 330

layout(location = 0) in vec3 position;

layout(std140) uniform viewMatrices
{
	uniform mat4 modelToCameraMatrix;
	uniform mat4 cameraToClipMatrix;
};

out vec3 texCoord;

void main()
{
	texCoord = position;
	gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));
}