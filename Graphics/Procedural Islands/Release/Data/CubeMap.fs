#version 330

in vec3 texCoord;

out vec4 outputColor;

uniform samplerCube skySampler;

void main ()
{
	outputColor = texture(skySampler, texCoord);
}