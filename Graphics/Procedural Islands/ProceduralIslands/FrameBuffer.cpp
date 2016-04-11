#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(int w, int h)
{
	width = w;
	height = h;

	glGenFramebuffers(1, &frameBufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLenum colorBuffer = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &colorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);
}

void FrameBuffer::BindTexture(Texture* colorBuffer)
{

	if (colorBuffer == NULL)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer->GetTexHandle(), 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteFramebuffers(1, &frameBufferHandle);
}