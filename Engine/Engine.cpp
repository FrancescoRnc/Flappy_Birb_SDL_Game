#include <iostream>
#include "Engine.h"

Engine* Engine::_instance = nullptr;

Engine::~Engine()
{
	std::cout << "Destroying everything..." << std::endl;
	delete(fileMgr);
	delete(componentMgr);
	delete(renderMgr);
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
}

int Engine::Initialize()
{
	// Initialization
	std::cout << "Initialization..." << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO/*| 
				 SDL_INIT_AUDIO | 
				 SDL_INIT_GAMECONTROLLER | 
				 SDL_INIT_TIMER*/))
	{
		std::cout << "Error during Initialization" << std::endl;
		return 1;
	}
	std::cout << "Initialization Succesfull!" << std::endl;
	std::cout << std::endl;

	/*// Window
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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | 
								              SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cout << "Error during Renderer Creation" << std::endl;
		return 1;
	}
	std::cout << "Renderer Created Succesfully!" << std::endl;
	std::cout << std::endl;*/

	renderMgr->Initialize();

	// FileManager
	std::cout << "Loading Images..." << std::endl;
	fileMgr->LoadTextures("./Assets/sprites/", renderMgr->Renderer);
	std::cout << "Loading Audio tracks..." << std::endl;
	fileMgr->LoadAudio("./Assets/audio/");

	// Input Handler
	//InputHandler = new KeyMouseInputHandler();
	//InputHandler->Initialize();
	KeyboardHandler = new KeyMouseInputHandler();
	if (KeyboardHandler->Initialize()) return 1;

	current_scene->LoadObjects();


	auto c = reinterpret_cast<FlapComponent*>(ComponentManager::Get()->
											  ComponentMap["Flap"][0]);
	KeyboardHandler->InputMap.insert({c->KeyCode, c->DoFlap});

	std::cout << "Initialization Completed!" << std::endl;

	return 0;
}

int Engine::Loop()
{
	//float span = 0.;
	int ret_value = 0;
	double dt = 0;
	std::cout << "Looping..." << std::endl;
	for (;;)
	{
		dt = clock->Tick();
		if (KeyboardHandler->Loop()) return 1;
		Update(dt);
		if (renderMgr->Draw()) return 1;
	}

	return ret_value;
}

int Engine::Update(const double deltatime)
{
	current_scene->UpdateComponents(deltatime, componentMgr->ComponentMap);
	return 0;
}
/*void Engine::Draw()
{
	SDL_SetRenderDrawColor(renderMgr->Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderMgr->Renderer);

	//current_scene->DrawComponents(renderer, componentMgr->ComponentMap["Sprite"]);
	renderMgr->Draw();

	SDL_RenderPresent(renderMgr->Renderer);
}*/

void Engine::OnExitGame()
{
	std::cout << "Exiting the Game..." << std::endl << std::endl;
}
