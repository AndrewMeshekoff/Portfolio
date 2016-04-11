#version 330

smooth in vec3 texCoord;

layout(location = 0) out vec4 outputColor;

uniform samplerCube skySampler;

void main ()
{
	outputColor = texture(skySampler, texCoord) * vec4(1.0, 0.9, 0.9, 1.0);
}