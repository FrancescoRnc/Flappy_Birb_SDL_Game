#pragma once
#include <cstdint>
#include <SDL.h>

struct Clock
{
	Clock();
	Clock(const Clock&) = delete;
	Clock(Clock&&) = delete;
	Clock& operator=(const Clock&) = delete;
	Clock& operator=(Clock&&) = delete;
	~Clock();

	double Tick();
	void Set_Now();
	double GetDeltaTime();

	private:
	int fps;
	std::uint64_t then, now;
	double deltatime;
}; 

