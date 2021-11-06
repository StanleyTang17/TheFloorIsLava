#pragma once

#ifndef MOUSE_SCROLL_INPUT_H
#define MOUSE_SCROLL_INPUT_H

class MouseMoveInput
{
public:
	virtual ~MouseMoveInput() {}
	virtual void handle_mouse_move_input(const float dt, const double offset_x, const double offset_y) = 0;
};

#endif