#ifndef TEXTURE_H
#define TEXTURE_H
#include <glload/gl_3_3.h>

class Texture
{
private:
	GLuint textureHandle;
	GLuint samplerHandle;
	GLenum type;

	void LoadCubeSide(const char* textureFile, GLenum side);
public:
	unsigned int width;
	unsigned int height;
	Texture(const char * filePath);
	Texture(const char* front,
		const char* back,
		const char* top,
		const char* bottom,
		const char* left,
		const char* right);
	Texture(int w, int h, GLenum format = GL_RGBA);
	GLuint GetTexHandle();
	void BindTexture(GLuint target);
	~Texture();
};






#endif