#ifndef SHADER_H
#define SHADER_H
#include <vector>
#include <map>
#include <glload/gl_3_3.h>

enum UniformEnums
{
	VIEW_BLOCK_INDEX,
	MOD_TO_CAM,
	MOD_TO_WORLD,
	NORMAL_MOD_TO_CAM,
	CAM_TO_CLIP,
	CLIP_HEIGHT,
	LIGHT_DIR,
	GRASS_TEX_TARGET,
	SAND_TEX_TARGET,
	SKY_TEX_TARGET,
	EYE_TAN_SPACE,
	LIGHT_TAN_SPACE,
	REFLECT_TEX_TARGET,
	REFRACT_TEX_TARGET,
	CLIP_TO_TEX,
	DISPLACE_TEX_TARGET,
	NORMAL_TEX_TARGET,
	FLOOR_DEPTH_TARGET,
	SURF_DEPTH_TARGET,
	TIME_1,
	TIME_2
};

class ShaderProgram
{
protected:
	std::vector<GLuint> shaderList;
	GLuint program;

	virtual void GetUniformVariables();

public:
	std::map<UniformEnums, GLuint> uniforms;

	ShaderProgram();

	void LoadShader(GLenum shaderType, char * shaderFileName);

	void LinkProgram();

	GLuint GetProgramHandle();

	virtual ~ShaderProgram();
};

class IslandShader : public ShaderProgram
{
	void GetUniformVariables();
public:
	IslandShader();
};

class IslandReflectShader : public ShaderProgram
{
	void GetUniformVariables();
public:
	IslandReflectShader();
};

class SkyboxShader : public ShaderProgram
{
	void GetUniformVariables();
public:
	SkyboxShader();
};

class AlphaShader : public ShaderProgram
{
	void GetUniformVariables();
public:
	AlphaShader();
};

class WaterShader : public ShaderProgram
{
	void GetUniformVariables();
public:
	WaterShader();
};

#endif
