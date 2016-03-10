#ifndef SHADER_H
#define SHADER_H
#include <vector>
#include <map>
#include <glload/gl_3_3.h>

enum UniformEnums
{
	VIEW_BLOCK_INDEX,
	MOD_TO_CAM,
	NORMAL_MOD_TO_CAM,
	CAM_TO_CLIP,
	LIGHT_DIR,
	GRASS_TEX_TARGET,
	SAND_TEX_TARGET,
	SKY_TEX_TARGET
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

class SkyboxShader : public ShaderProgram
{
	void GetUniformVariables();
public:
	SkyboxShader();
};


#endif
