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

	virtual void init_texture() {};
	virtual void init_renderbuffer() {};
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
public:
	ScreenFramebuffer(int width, int height);
};

class DepthFramebuffer : public Framebuffer
{
protected:
	void init_texture();
public:
	DepthFramebuffer(int width, int height);
};

class DepthCubeFramebuffer : public Framebuffer
{
protected:
	void init_texture();
public:
	DepthCubeFramebuffer(int width, int height);
};

class HDRFramebuffer : public Framebuffer
{
protected:
	void init_texture();
	void init_renderbuffer();
public:
	HDRFramebuffer(int width, int height, bool use_depth = true);
};

class BloomFramebuffer : public Framebuffer
{
protected:
	GLuint bright_texture;

	void init_texture();
	void init_renderbuffer();
public:
	BloomFramebuffer(int width, int height);

	inline GLuint get_bright_texture() const { return this->bright_texture; }
};

#endif