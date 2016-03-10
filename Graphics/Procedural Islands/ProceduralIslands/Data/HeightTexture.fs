#version 330

smooth in vec4 interpLight;
in vec3 texCoord;

out vec4 outputColor;

uniform sampler2D grassSampler;
uniform sampler2D sandSampler;

void main()
{
	float upperLimit = 0.7;
	float lowerLimit = 0.6;

	vec2 texCoordOffset = vec2(texCoord.x + 0.5, texCoord.z + 0.5);

	float alpha = clamp(texCoord.y - lowerLimit, 0, upperLimit - lowerLimit) / (upperLimit - lowerLimit);

	outputColor = mix(texture2D(sandSampler, texCoordOffset.st), texture2D(grassSampler, texCoordOffset.st), alpha) * interpLight;
}