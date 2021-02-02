#include <iostream>
#include "Engine.h"


Engine::~Engine()
{
	std::cout << "Destroying everything..." << std::endl;
	delete(fileMgr);
	delete(componentMgr);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}



int Engine::Init()
{
	// Initialization
	std::cout << "Initialization..." << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO | 0))
	{
		std::cout << "Error during Initialization" << std::endl;
		return 1;
	}
	std::cout << "Initialization Succesfull!" << std::endl;
	std::cout << std::endl;

	// Window
	std::cout << "Creating Window..." << std::endl;
	window = SDL_CreateWindow(WindowInfo.Title, WindowInfo.ScreenX, WindowInfo.ScreenY, WindowInfo.Width, WindowInfo.Height, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cout << "Error during Window Creation" << std::endl;
		return 1;
	}
	std::cout << "Window Created Succesfully!" << std::endl;
	std::cout << std::endl;

	// Renderer
	std::cout << "Creating Renderer..." << std::endl;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cout << "Error during Renderer Creation" << std::endl;
		return 1;
	}
	std::cout << "Renderer Created Succesfully!" << std::endl;
	std::cout << std::endl;

	// FileManager
	std::cout << "Loading Images..." << std::endl;
	fileMgr->LoadTextures("./Assets/sprites/", renderer);
	std::cout << "Loading Audio tracks..." << std::endl;
	fileMgr->LoadAudio("./Assets/audio/");

	// Input Handler
	InputHandler = new KeyMouseInputHandler();
	InputHandler->InitializeInput();

	current_scene->LoadObjects();

	std::cout << "Initialization Completed!" << std::endl;

	return 0;
}
int Engine::Loop()
{
	//float span = 0.;
	int ret_value = 0;
	float dt = 0;
	std::cout << "Looping..." << std::endl;
	for (;;)
	{
		dt = clock->Tick();
		if (InputHandler->Loop())	return 1;
		Update(dt);
		Draw();
	}

	return ret_value;
}

void Engine::Update(const float deltatime)
{
	current_scene->UpdateComponents(deltatime, componentMgr->ComponentMap);
}
void Engine::Draw()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	current_scene->DrawComponents(renderer, componentMgr->ComponentMap["Sprite"]);

	SDL_RenderPresent(renderer);
}

void Engine::OnExitGame()
{
	std::cout << "Exiting the Game..." << std::endl << std::endl;
}
