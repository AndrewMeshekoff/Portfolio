#version 330

layout(location = 0) out vec4 outputColor;

//Texture Samplers
uniform sampler2D normalSampler;
uniform sampler2D displaceSampler;
uniform sampler2D reflectSampler;
uniform sampler2D refractSampler;
uniform sampler2D floorDepthSampler;
uniform sampler2D surfDepthSampler;

uniform vec3 lightTanSpace;

//timing
uniform float texOffset1;
uniform float texOffset2;

smooth in vec3 modelPosition;
in vec4 clipTexProjCoord;
smooth in vec3 eyeDirection;

vec2 texProjCoord;

float dblDispTiling = 2.0;
float normalTiling = 2.0;


vec2 GetMapCoord(in vec2 offset, in float scale)
{
	return vec2(fract(modelPosition.x * scale + offset.x), fract(modelPosition.z * scale + offset.y));
}

vec4 GetRefractColor(in vec2 displacedCoord)
{
	vec4 color = texture2D(refractSampler, displacedCoord);
	if (color.a == 1.0)
		return color;
	return texture2D(refractSampler, texProjCoord);
}

float GetWaterDepth(in vec2 displacedCoord)
{
	float waterDepth = ((1.0/(1.0 - texture2D(floorDepthSampler, displacedCoord).r)) - (1.0/(1.0 - texture2D(surfDepthSampler, displacedCoord).r))) * 0.1;
	return clamp(waterDepth, 0.001, 0.999);
}

void main()
{
	vec2 offsetVec1 = vec2(0.0, texOffset1);
	vec2 offsetVec2 = vec2(0.0, -texOffset2);

	//texture projection coordinates
	texProjCoord.x = clipTexProjCoord.x / clipTexProjCoord.w;
	texProjCoord.y = clipTexProjCoord.y / clipTexProjCoord.w;

	//double displacement coordinates
	vec2 doubleDispMapCoord = texture2D(displaceSampler, GetMapCoord(offsetVec2, dblDispTiling)).rg * 0.1;
	vec2 dispMapCoord = GetMapCoord(doubleDispMapCoord + offsetVec1, normalTiling);
	vec2 displacement = texture2D(displaceSampler, dispMapCoord).rg;
	displacement = ((displacement - vec2(0.5)) * 2) * 0.1;

	vec2 displacedCoord = texProjCoord + displacement;
	displacedCoord = clamp(displacedCoord, 0.001, 0.999);

	//vec2 depthCoord = displacedCoord;
	vec4 refractColor = texture2D(refractSampler, displacedCoord);
	if (refractColor.a != 1.0)
	{
		refractColor = texture2D(refractSampler, texProjCoord);
		displacedCoord = texProjCoord;
	}

	//water depth
	float waterDepth = GetWaterDepth(displacedCoord);
	float invWaterDepth = 1.0 - waterDepth;
	
	//displaced normal
	vec2 normalMapCoord = GetMapCoord(doubleDispMapCoord + offsetVec1, normalTiling);
	vec2 reverseMapCoord = GetMapCoord(doubleDispMapCoord + offsetVec1, normalTiling * 4);
	vec3 normal = texture2D(normalSampler, normalMapCoord).rbg;
	vec3 reverseMapNormal = texture2D(normalSampler, vec2(1.0)-reverseMapCoord).rbg;
	normal = normalize((normal - vec3(0.5)) + (reverseMapNormal - vec3(0.5)));

	//specular light
	vec3 eyeNormDirection = normalize(eyeDirection);
	vec3 halfAngle = normalize(lightTanSpace + eyeNormDirection);
	float specular = pow(max(dot(halfAngle, normal), 0), 100);

	//fresnel
	float invFresnel = max(dot(normal, eyeNormDirection), 0);
	float fresnel = 1.0 - invFresnel;

	vec4 reflectColor = texture2D(reflectSampler, displacedCoord);
	
	outputColor = (refractColor * invWaterDepth * invFresnel) + reflectColor * fresnel + (normalize(vec4(0.3, 0.5, 0.8, 0.0)) * waterDepth * invFresnel) + vec4(0.8, 0.5, 0.5, 1.0) * specular;
}