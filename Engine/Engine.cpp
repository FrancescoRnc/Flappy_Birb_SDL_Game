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

	// Render Manager
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

	// Scene
	auto player = new PlayerObjPack();
	std::vector<PipesPairObjPack*> pipesPairs = {
		new PipesPairObjPack(300, 0),
		new PipesPairObjPack(450, 0),
		new PipesPairObjPack(600, 0),
	};
	ScoreObjPack* score = new ScoreObjPack(pipesPairs);
	ScoreCounterObjPack* scoreCounter = new ScoreCounterObjPack();
	ScoreBridge* Sbridge = new ScoreBridge();
	Sbridge->refScore = score;
	Sbridge->refCounter = scoreCounter;
	score->Bridge = Sbridge;
	scoreCounter->Bridge = Sbridge;
	current_scene->Load(
		{
			player,
			pipesPairs[0],
			pipesPairs[1],
			pipesPairs[2],
			score,
			scoreCounter,
			new BackgroundObjPack()
		});


	auto c = reinterpret_cast<FlapComponent*>(ComponentManager::Get()->
											  ComponentMap["Flap"][0]);
	KeyboardHandler->InputMap.insert({c->KeyCode, c->DoFlap});
	auto OneShotFunction = [this, player, pipesPairs]()
	{
		pipesPairs[0]->Movement->bActive = true;
		pipesPairs[1]->Movement->bActive = true;
		pipesPairs[2]->Movement->bActive = true;
		player->Flap->DoFlap->Action();
		std::cout<<"Game Start!"<<std::endl;
		KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.pop();
	};
	KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.push(OneShotFunction);

	std::cout << "Initialization Completed!" << std::endl;

	return 0;
}

int Engine::Loop()
{
	double dt = 0;
	std::cout << "Looping..." << std::endl;
	for (;;)
	{
		dt = clock->Tick();
		if (KeyboardHandler->Loop()) 
			return 1;
		Update(dt);
		if (renderMgr->Draw()) 
			return 1;
	}

	return 0;
}

int Engine::Update(const double deltatime)
{
	ComponentManager::Get()->Update(deltatime);
	current_scene->Update(deltatime);
	return 0;
}

void Engine::OnExitGame()
{
	std::cout << "Exiting the Game..." << std::endl << std::endl;
}
