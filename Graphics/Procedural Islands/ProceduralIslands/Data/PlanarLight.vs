#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

smooth out vec4 interpLight;
out vec3 texCoord;

uniform vec3 dirToLight;

layout(std140) uniform viewMatrices
{
	uniform mat4 modelToCameraMatrix;
	uniform mat4 cameraToClipMatrix;
};

uniform mat3 normalModelToCameraMatrix;

void main()
{
	gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

	vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
	
	float cosAngIncidence = dot(normCamSpace, dirToLight);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	interpLight = vec4(1.0, 1.0, 1.0, 1.0) * cosAngIncidence;
	texCoord = position;
}