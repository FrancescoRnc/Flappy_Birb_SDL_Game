#pragma once
#include <SDL.h>
#include <iostream>
#include <functional>
#include "ComponentManager.h"

class IInputHandler
{
	public:

	virtual int InitializeInput() = 0;

	virtual int Loop() = 0;
};

class KeyMouseInputHandler : public IInputHandler
{
	public:

	virtual int InitializeInput() override;

	virtual int Loop() override;
};

class JoystickInputHandler : public IInputHandler
{
	public:

	virtual int InitializeInput() override;

	virtual int Loop() override;
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

