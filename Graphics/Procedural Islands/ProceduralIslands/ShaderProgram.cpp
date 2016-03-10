
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "ShaderProgram.h"
#include "ProgramData.h"
#include <glload/gl_3_3.h>

ShaderProgram::ShaderProgram()
	:shaderList()
{
	program = 0;
}

void ShaderProgram::LoadShader(GLenum shaderType, char * shaderFileName)
{
	int nameSize = 0;
	int pathSize = 0;

	for (nameSize = 0; shaderFileName[nameSize] != '\0'; nameSize++)
	{}

	for (pathSize = 0; DATA_PATH[pathSize] != '\0'; pathSize++)
	{}

	char * shaderFilePath = new char[nameSize + pathSize + 1];
	std::strcpy(shaderFilePath, DATA_PATH);
	std::strcpy(shaderFilePath + pathSize, shaderFileName);

	std::ifstream shaderFile(shaderFilePath);

	shaderFile.seekg(0, shaderFile.end);
	unsigned int fileSize = shaderFile.tellg();

	char * glslSource = new char[fileSize + 1];
	memset(glslSource, 0, fileSize);

	shaderFile.seekg(0);
	shaderFile.read(glslSource, fileSize);
	glslSource[fileSize] = '\0';
	GLchar** shaderSource = &glslSource;

	GLuint shaderHandle = glCreateShader(shaderType);
	glShaderSource(shaderHandle, 1, shaderSource, NULL);
	glCompileShader(shaderHandle);

	GLint result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint maxLength;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		char *outputLog = new char[maxLength + 1];
		glGetShaderInfoLog(shaderHandle, maxLength, NULL, outputLog);

		switch (shaderType)
		{
			case GL_VERTEX_SHADER:
				printf("vertex shader compilation error:\n"); 
				break;
			case GL_GEOMETRY_SHADER:
				printf("geometry shader compilation error:\n");
				break;
			case GL_FRAGMENT_SHADER:
				printf("fragment shader compilation error:\n");
				break;
		}

		fprintf(stderr, outputLog);
		delete[] outputLog;
	}
	else
		shaderList.push_back(shaderHandle);

	delete[] shaderFilePath;
	delete[] glslSource;
}

void ShaderProgram::LinkProgram()
{
	program = glCreateProgram();

	for (auto it = shaderList.begin(); it != shaderList.end(); it++)
	{
		glAttachShader(program, *it);
	}

	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		char * compilationResult = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, &maxLength, compilationResult);

		glDeleteProgram(program);
		for (auto it = shaderList.begin(); it != shaderList.end(); it++)
		{
			glDeleteShader(*it);
		}

		fprintf(stderr, compilationResult);

		exit(0);
	}

	for (auto it = shaderList.begin(); it != shaderList.end(); it++)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	
	shaderList.clear();

	GetUniformVariables();
}

void ShaderProgram::GetUniformVariables()
{
}

GLuint ShaderProgram::GetProgramHandle()
{
	return program;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

IslandShader::IslandShader()
	:ShaderProgram()
{}

void IslandShader::GetUniformVariables()
{
	uniforms[UniformEnums::VIEW_BLOCK_INDEX] = glGetUniformBlockIndex(program, "viewMatrices");
	uniforms[UniformEnums::MOD_TO_CAM] = glGetUniformLocation(program, "modelToCameraMatrix");
	uniforms[UniformEnums::NORMAL_MOD_TO_CAM] = glGetUniformLocation(program, "normalModelToCameraMatrix");
	uniforms[UniformEnums::LIGHT_DIR] = glGetUniformLocation(program, "dirToLight");
	uniforms[UniformEnums::CAM_TO_CLIP] = glGetUniformLocation(program, "cameraToClipMatrix");
	uniforms[UniformEnums::GRASS_TEX_TARGET] = glGetUniformLocation(program, "grassSampler");
	uniforms[UniformEnums::SAND_TEX_TARGET] = glGetUniformLocation(program, "sandSampler");
}

SkyboxShader::SkyboxShader()
	:ShaderProgram()
{}

void SkyboxShader::GetUniformVariables()
{
	uniforms[UniformEnums::VIEW_BLOCK_INDEX] = glGetUniformBlockIndex(program, "viewMatrices");
	uniforms[UniformEnums::SKY_TEX_TARGET] = glGetUniformLocation(program, "skySampler");
}