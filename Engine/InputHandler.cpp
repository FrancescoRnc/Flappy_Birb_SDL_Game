#include "InputHandler.h"

// Keyboard and Mouse
int KeyMouseInputHandler::InitializeInput()
{
	std::cout << "Initializing Keyboard & Mouse Input..." << std::endl;

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

			if (ev.key.keysym.sym == SDLK_SPACE)
			{
				auto c = reinterpret_cast<FlapComponent*>(ComponentManager::Get()->ComponentMap["Flap"][0]);
				c->DoFlap->Action();
			}
			
		}

	}
	return 0;
}


// Joystick
int JoystickInputHandler::InitializeInput()
{
	std::cout << "Initializing Joystick Input..." << std::endl;

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
	}
	return 0;
}
