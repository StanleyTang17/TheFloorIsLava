#pragma once

#include"OPENGL.h"
#include"NATIVE.h"

class Framebuffer
{
protected:
	const int WIDTH;
	const int HEIGHT;

	GLuint id;
	GLuint texture;
	GLuint renderbuffer;
	
	GLenum texture_type;
	GLenum renderbuffer_type;

	virtual void init_texture() = 0;
	virtual void init_renderbuffer() = 0;
	void init();

public:
	Framebuffer(GLenum texture_type, GLenum renderbuffer_type, int width, int height);
	~Framebuffer();

	void bind(bool clear);
	void blit(Framebuffer* framebuffer, GLbitfield buffer_mask, GLenum filter);
	GLuint get_texture() const { return texture; }

	static void bind_default();
};

class MultiSampleFramebuffer : public Framebuffer
{
private:
	int num_samples;
protected:
	void init_texture();
	void init_renderbuffer();
public:
	MultiSampleFramebuffer(int width, int height, int num_samples);
};

class ScreenFramebuffer : public Framebuffer
{
protected:
	void init_texture();
	void init_renderbuffer();
public:
	ScreenFramebuffer(int width, int height);
};