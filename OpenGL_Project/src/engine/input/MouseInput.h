#pragma once

#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H

#include"libs/OPENGL.h"

class MouseInput
{
public:
	virtual ~MouseInput() {};
	virtual void handle_mouse_input(GLFWwindow* window, int button, int action) = 0;
};

#endif