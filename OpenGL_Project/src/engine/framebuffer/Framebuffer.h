#pragma once

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include"libs/OPENGL.h"
#include"libs/NATIVE.h"

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
	GLuint get_texture() const { return this->texture; }
	int get_width() const { return this->WIDTH; }
	int get_height() const { return this->HEIGHT; }

	static void bind_default(bool clear);
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

class DepthFramebuffer : public Framebuffer
{
protected:
	void init_texture();
	void init_renderbuffer();
public:
	DepthFramebuffer(int width, int height);
};

class DepthCubeFramebuffer : public Framebuffer
{
protected:
	void init_texture();
	void init_renderbuffer();
public:
	DepthCubeFramebuffer(int width, int height);
};

#endif