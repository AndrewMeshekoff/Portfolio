#version 330

smooth in vec4 interpLight;
smooth in vec3 texCoord;
smooth in float fragHeight;

layout(location = 0) out vec4 outputColor;

uniform float clipHeight;

uniform sampler2D grassSampler;
uniform sampler2D sandSampler;

void main()
{
	if (fragHeight > clipHeight + 0.01)
	{
		discard;
	}

	float upperLimit = 0.62;
	float lowerLimit = 0.52;

	vec2 texCoordOffset = vec2(texCoord.x + 0.5, texCoord.z + 0.5);

	float alpha = clamp(texCoord.y - lowerLimit, 0, upperLimit - lowerLimit) / (upperLimit - lowerLimit);

	vec2 texCoordTiled = vec2(fract(texCoordOffset.x * 2), fract(texCoordOffset.y * 2));

	outputColor = mix(texture2D(sandSampler, texCoordTiled), texture2D(grassSampler, texCoordTiled), alpha) * (interpLight * 0.8 + vec4(0.8, 0.6, 0.4, 0.0) * 0.2);
}