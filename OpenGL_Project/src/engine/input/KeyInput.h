#pragma once

#ifndef KEY_INPUT_H
#define KEY_INPUT_H

#include"libs/OPENGL.h"

class KeyInput
{
public:
	virtual ~KeyInput() {}
	virtual void handle_key_input(GLFWwindow* window, int key, int action) = 0;
};

#endif