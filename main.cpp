#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include "Engine/Engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Engine/stb_image.h"

#define CHECK(I) if (I) goto end;

int main()
{
	Engine* engine = new Engine();

	// If Initialization or Loop return, then the program ends

	CHECK(engine->Initialize());
	CHECK(engine->Loop());
	
	end:
	std::cout << std::endl;
	engine->OnExitGame();
	delete(engine);
	return 0;
}