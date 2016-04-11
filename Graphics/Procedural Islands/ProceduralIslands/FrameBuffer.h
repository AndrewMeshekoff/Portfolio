#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H
#include <glload/gl_3_3.h>
#include "Texture.h"

struct FrameBuffer
{
	GLuint frameBufferHandle;
	GLuint depthBuffer;

	int width;
	int height;

	FrameBuffer(int w, int h);
	void BindBuffer();
	void BindTexture(Texture* colorBuffer);


	~FrameBuffer();
};




#endif