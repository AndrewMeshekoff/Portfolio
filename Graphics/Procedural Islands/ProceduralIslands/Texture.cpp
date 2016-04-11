#include <glimg/glimg.h>
#include <stb_image.h>
#include "Texture.h"
#include "ProgramData.h"

extern ProgramData ProgramContext;

Texture::Texture(const char * textureFile)
{
	type = GL_TEXTURE_2D;

	int x, y, n;
	unsigned char* imageData = stbi_load(textureFile, &x, &y, &n, 4);
	width = x;
	height = y;

	glGenTextures(1, &textureHandle);
	glBindTexture(type, textureHandle);
	glTexImage2D(type, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);


	glGenSamplers(1, &samplerHandle);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete[] imageData;
}

Texture::Texture(const char* front,
				const char* back,
				const char* top,
				const char* bottom,
				const char* left,
				const char* right)
{
	type = GL_TEXTURE_CUBE_MAP;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);

	LoadCubeSide(front, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	LoadCubeSide(back, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	LoadCubeSide(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	LoadCubeSide(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	LoadCubeSide(left, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	LoadCubeSide(right, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glGenSamplers(1, &samplerHandle);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::LoadCubeSide(const char* textureFile, GLenum side)
{
	int x, y, n;
	unsigned char* imageData = stbi_load(textureFile, &x, &y, &n, 4);
	width = x;
	height = y;

	glTexImage2D(side,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		imageData
		);

	delete[] imageData;
}


Texture::Texture(int w, int h, GLenum format)
{
	width = w;
	height = h;

	type = GL_TEXTURE_2D;
	glGenTextures(1, &textureHandle);
	glBindTexture(type, textureHandle);

	glTexImage2D(type, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, 0);

	glGenSamplers(1, &samplerHandle);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GLuint Texture::GetTexHandle()
{
	return textureHandle;
}

void Texture::BindTexture(GLuint target)
{
	glActiveTexture(GL_TEXTURE0 + target);
	glBindTexture(type, textureHandle);
	glBindSampler(target, samplerHandle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandle);
	glDeleteSamplers(1, &samplerHandle);
}