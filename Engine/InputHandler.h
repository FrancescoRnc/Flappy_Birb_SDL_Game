#pragma once
#include <SDL.h>
#include <iostream>
#include <functional>
#include <map>
#include <stack>
#include "ComponentManager.h"
#include "Interfaces.h"

// These loads and behave for Keyboard use
class KeyMouseInputHandler : public IInputHandler
{
	public:

	std::map<SDL_KeyCode, InputAction*> InputMap;

	virtual int Initialize() override;

	virtual int Loop() override;
};

// These loads and behave for Keyboard use
class JoystickInputHandler : public IInputHandler
{
	public:

	virtual int Initialize() override;

	virtual int Loop() override;
};

// To use for any kind of button
struct InputAction
{
	std::stack<std::function<void()>> ActionStack;

	void operator() ()
	{
		ActionStack.top();
	}
};

// To use for Joypad analog sticks andf other Axis-able Input
struct InputAxis
{
	std::function<void(float)> Action;

	void operator() (float value)
	{
		Action(value);
	}
};

