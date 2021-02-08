#pragma once
#include <SDL.h>
#include <iostream>
#include <functional>
#include <map>
#include "ComponentManager.h"
#include "Interfaces.h"


class KeyMouseInputHandler : public IInputHandler
{
	public:

	std::map<SDL_KeyCode, InputAction*> InputMap;

	virtual int Initialize() override;

	virtual int Loop() override;
};

class JoystickInputHandler : public IInputHandler
{
	public:

	//std::map<, std::function<void()>> InputMap;

	virtual int Initialize() override;

	virtual int Loop() override;
};

struct InputContainer
{
	virtual void Insert() = 0;
};


struct InputAction
{
	std::function<void()> Action = {};

	void operator() ()
	{
		Action();
	}
};

struct InputAxis
{
	std::function<void(float)> Action;

	void operator() (float value)
	{
		Action(value);
	}
};

