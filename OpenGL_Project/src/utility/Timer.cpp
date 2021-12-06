#include "Timer.h"

Timer::Timer(float tick_interval)
{
	this->tick_interval = tick_interval;
	this->last_tick_time = glfwGetTime();
	this->init_time = glfwGetTime();
	this->ticks = 0;
}

bool Timer::tick()
{
	float current_time = glfwGetTime();
	if (current_time - this->last_tick_time >= this->tick_interval)
	{
		++ticks;
		this->last_tick_time = current_time;
		return true;
	}
	else
	{
		return false;
	}
}

void Timer::reset()
{
	this->init_time = glfwGetTime();
	this->last_tick_time = init_time;
	this->ticks = 0;
}