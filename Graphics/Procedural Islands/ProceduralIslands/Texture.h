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
	Texture(const char * filePath);
	Texture(const char* front,
		const char* back,
		const char* top,
		const char* bottom,
		const char* left,
		const char* right);
	void BindTexture(GLuint target);
	~Texture();
};






#endif