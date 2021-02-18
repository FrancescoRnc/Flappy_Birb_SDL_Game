#include "InputHandler.h"

// Keyboard and Mouse
int KeyMouseInputHandler::Initialize()
{
	std::cout << "Initializing Keyboard & Mouse Input..." << std::endl;

	// TODO

	std::cout << "Initialization completed!" << std::endl << std::endl;

	return 0;
}

int KeyMouseInputHandler::Loop()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_QUIT)
			return 1;
		if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				return 1;
			for (auto action : InputMap)
			{
				if (ev.key.keysym.sym == action.first)
				{
					if (!action.second->ActionStack.empty())
						action.second->ActionStack.top()();
					//else
					//	std::cout<<"No Input Action"<<std::endl;
				}
			}
		}
	}
	return 0;
}


// Joystick
int JoystickInputHandler::Initialize()
{
	std::cout << "Initializing Joystick Input..." << std::endl;

	// TODO

	std::cout << "Initialization completed!" << std::endl << std::endl;

	return 0;
}

int JoystickInputHandler::Loop()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_QUIT)
			return 1;

		// TODO
	}
	return 0;
}
