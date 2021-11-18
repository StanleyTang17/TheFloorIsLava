#pragma once

#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include"libs/OPENGL.h"

class Timer
{
private:
	float init_time;
	float last_tick_time;
	float tick_interval;
	int ticks;
public:
	Timer(float tick_interval = 1.0f);
	
	bool tick();
	inline int get_ticks() const { return this->ticks; }
	inline void set_ticks(int ticks) { this->ticks = ticks; }
	inline float get_total_time_elapsed() const { return glfwGetTime() - this->init_time; }
	inline void reset_total_time() { this->init_time = glfwGetTime(); }
};

#endif