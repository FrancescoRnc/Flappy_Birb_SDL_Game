#include <iostream>
#include "Engine.h"

Engine* Engine::_instance = nullptr;

Engine::Engine()
{
	_instance = this;
	fileMgr = new FileManager();
	audioSys = new AudioSystem();
	componentMgr = new ComponentManager();
	renderMgr = new RenderManager({"Flappy Birb",
								  SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED,
								  270, 480});
	clock = new Clock();
	current_scene = new Scene();
}

Engine::~Engine()
{
	std::cout << "Destroying everything..." << std::endl;
	delete(fileMgr);
	delete(audioSys);
	delete(componentMgr);
	delete(renderMgr);
}

int Engine::Initialize()
{
	// Initialization
	std::cout << "Initialization..." << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO| 
				 SDL_INIT_AUDIO/* | 
				 SDL_INIT_GAMECONTROLLER | 
				 SDL_INIT_TIMER*/))
	{
		std::cout << "Error during Initialization" << std::endl;
		return 1;
	}
	std::cout << "Initialization Succesfull!" << std::endl;
	std::cout << std::endl;

	// Render Manager Initialization
	if (renderMgr->Initialize()) return 1;
	if (audioSys->Initialize()) return 1;

	// File Manager Initialization
	std::cout << "Loading Images..." << std::endl;
	if (fileMgr->LoadTextures("./Assets/sprites/", renderMgr->Renderer)) 
		return 1;

	// Audio System Initialization
	std::cout << "Loading Audio files..." << std::endl;
	if (audioSys->LoadAudioFiles("./Assets/audio/")) return 1;

	// Input Handler Initialization
	KeyboardHandler = new KeyMouseInputHandler();
	if (KeyboardHandler->Initialize()) return 1;

	// Scene

	// Here you can put whatever you want for your game
	auto player = new PlayerObjPack();
	std::vector<PipesPairObjPack*> pipesPairs = {
		new PipesPairObjPack(300, 0),
		new PipesPairObjPack(450, 0),
		new PipesPairObjPack(600, 0),
	};
	ScoreObjPack* score = new ScoreObjPack(pipesPairs);
	ScoreCounterObjPack* scoreCounter = new ScoreCounterObjPack();
	ScoreBridge* Sbridge = new ScoreBridge(score, scoreCounter);
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

	auto flap = reinterpret_cast<FlapComponent*>(ComponentManager::Get()->
											  ComponentMap["Flap"][0]);
	KeyboardHandler->InputMap.insert({flap->KeyCode, flap->DoFlap});
	auto OneShotFunction = [this, player, pipesPairs]()
	{
		pipesPairs[0]->Movement->bActive = true;
		pipesPairs[1]->Movement->bActive = true;
		pipesPairs[2]->Movement->bActive = true;
		KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.pop();
		player->Flap->Func();
		player->Animator->SpeedRate = 3.;
	};
	KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.push(OneShotFunction);
	// - - - - - - - -

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
	int result = 0;
	result = ComponentManager::Get()->Update(deltatime);
	result = current_scene->Update(deltatime);
	return result;
}

void Engine::OnExitGame()
{
	std::cout << "Exiting the Game..." << std::endl << std::endl;
}
