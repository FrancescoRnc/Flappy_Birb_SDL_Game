#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include "Engine/Engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Engine/stb_image.h"

#define FULLHD_16_9_RESOLUTION 1920, 1080
#define FULLHD_9_16_RESOLUTION 1080, 1920
#define HD_16_9_RESOLUTION 1280, 720
#define HD_9_16_RESOLUTION 720, 1280
#define GAME_RESOLUTION 288, 512;

#define CHECK(I) if (I) goto end;

 WindowRectInfo wri{"Flappy Birb", 500, 50, 270, 480};

 SDL_Renderer* renderer;

 //FileManager* FileMgr;
 //ComponentManager* CompMgr;

int main()
{
	int return_value = 0;
	Engine* engine = new Engine(wri);

	CHECK(engine->Initialize());
	CHECK(engine->Loop());
	
	end:
	std::cout << std::endl;
	engine->OnExitGame();
	delete(engine);
	return 0;
}