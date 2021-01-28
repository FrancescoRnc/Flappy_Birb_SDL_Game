#include "Clock.h"
#include <iostream>

Clock::Clock()
{
	now = SDL_GetPerformanceCounter();
	then = now;
	fps = 60;
	deltatime = 0.;
}
Clock::~Clock()
{

}

double Clock::Tick()
{
	then = now;
	now = SDL_GetPerformanceCounter();
	return deltatime = (double(now - then) / SDL_GetPerformanceFrequency());
}
double Clock::GetDeltaTime()
{
	return deltatime;
}