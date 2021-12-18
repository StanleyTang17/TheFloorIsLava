#include "Framebuffer.h"

Framebuffer::Framebuffer(GLenum texture_type, GLenum renderbuffer_type, int width, int height)
	:
	WIDTH(width), HEIGHT(height)
{
	this->id = 0;
	this->texture = 0;
	this->renderbuffer = 0;
	this->texture_type = texture_type;
	this->renderbuffer_type = renderbuffer_type;
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &this->id);
}

void Framebuffer::init()
{
	glGenFramebuffers(1, &this->id);
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);

	if (this->texture_type)
	{
		glGenTextures(1, &this->texture);
		glBindTexture(texture_type, this->texture);
		this->init_texture();
	}

	if (this->renderbuffer_type)
	{
		glGenRenderbuffers(1, &this->renderbuffer);
		glBindRenderbuffer(renderbuffer_type, this->renderbuffer);
		this->init_renderbuffer();
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FRAMEBUFFER INIT FAILED" << std::endl;
		glfwTerminate();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind(bool clear)
{
	glViewport(0, 0, this->WIDTH, this->HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	if (clear)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}

void Framebuffer::bind_default(bool clear)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (clear)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}

void Framebuffer::blit(Framebuffer* other, GLbitfield buffer_mask, GLenum filter)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other->id);
	glBlitFramebuffer(0, 0, this->WIDTH, this->HEIGHT, 0, 0, other->WIDTH, other->HEIGHT, buffer_mask, filter);
}

MultiSampleFramebuffer::MultiSampleFramebuffer(int width, int height, int num_samples)
	:
	Framebuffer(GL_TEXTURE_2D_MULTISAMPLE, GL_RENDERBUFFER, width, height)
{
	this->num_samples = num_samples;
	this->init();
}

void MultiSampleFramebuffer::init_texture()
{
	glTexImage2DMultisample(this->texture_type, this->num_samples, GL_RGBA, this->WIDTH, this->HEIGHT, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture_type, this->texture, 0);
}

void MultiSampleFramebuffer::init_renderbuffer()
{
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->num_samples, GL_DEPTH24_STENCIL8, this->WIDTH, this->HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderbuffer);
}

ScreenFramebuffer::ScreenFramebuffer(int width, int height)
	:
	Framebuffer(GL_TEXTURE_2D, NULL, width, height)
{
	this->init();
}

void ScreenFramebuffer::init_texture()
{
	glTexImage2D(this->texture_type, 0, GL_RGBA, this->WIDTH, this->HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(this->texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture_type, this->texture, 0);
}

DepthFramebuffer::DepthFramebuffer(int width, int height)
	:
	Framebuffer(GL_TEXTURE_2D, NULL, width, height)
{
	this->init();
}

void DepthFramebuffer::init_texture()
{
	glTexImage2D(this->texture_type, 0, GL_DEPTH_COMPONENT, this->WIDTH, this->HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(this->texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(this->texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
	glTexParameteri(this->texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(this->texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

DepthCubeFramebuffer::DepthCubeFramebuffer(int width, int height)
	:
	Framebuffer(GL_TEXTURE_CUBE_MAP, NULL, width, height)
{
	this->init();
}

void DepthCubeFramebuffer::init_texture()
{
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, this->WIDTH, this->HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(this->texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(this->texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(this->texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

HDRFramebuffer::HDRFramebuffer(int width, int height)
	:
	Framebuffer(GL_TEXTURE_2D, GL_RENDERBUFFER, width, height)
{
	this->init();
}

void HDRFramebuffer::init_texture()
{
	glTexImage2D(this->texture_type, 0, GL_RGBA16F, this->WIDTH, this->HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(this->texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture_type, this->texture, 0);
}

void HDRFramebuffer::init_renderbuffer()
{
	glRenderbufferStorage(this->renderbuffer_type, GL_DEPTH_COMPONENT, this->WIDTH, this->HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->renderbuffer_type, this->renderbuffer);
}