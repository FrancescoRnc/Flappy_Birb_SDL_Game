#include <iostream>
#include "Engine.h"

Engine* Engine::_instance = nullptr;

Engine::Engine()
{
	_instance = this;
	fileMgr = new FileManager();
	renderMgr = new RenderManager({"Flappy Birb",
								  270, 480, // Screen size here
								  SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED});
	audioSys = new AudioSystem();
	componentMgr = new ComponentManager();
	gameEditor = new GameEditor();
	gameEditor->refEngine = this;
	current_scene = new Scene();
	clock = new Clock();
}

Engine::~Engine()
{
	std::cout << "Destroying everything..." << std::endl;
	delete(clock);  
	delete(current_scene);
	delete(gameEditor);
	delete(componentMgr);
	delete(audioSys);
	delete(renderMgr);
	delete(fileMgr);
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
	current_scene->Load(gameEditor->LoadObjects());

	KeyboardHandler->InputMap.insert({SDLK_SPACE, new InputAction()});
	KeyboardHandler->InputMap.insert({SDLK_BACKSPACE, new InputAction()});

	std::cout << "Initialization Completed!" << std::endl;

	return 0;
}


int Engine::Loop()
{
	double dt = 0;

	OnStartGame();

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

void Engine::OnStartGame()
{
	KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.push(gameEditor->player->Flap->Func);
	gameEditor->GameStart();
	std::cout<<"Game started!"<<std::endl<<std::endl;
}

void Engine::OnExitGame()
{
	std::cout << "Exiting the Game..." << std::endl << std::endl;
}

int GameEditor::GameStart()
{
	player->Locate(70, 200);
	*player->Rotator->Rotation = 0;
	player->Gravity->bActive = false;
	player->Animator->SpeedRate = 1.;
	player->Collision->bActive = true;

	pipesPairs[0]->Locate(300, 0);
	pipesPairs[0]->Movement->bActive = false;
	pipesPairs[1]->Locate(450, 0);
	pipesPairs[1]->Movement->bActive = false;
	pipesPairs[2]->Locate(600, 0);
	pipesPairs[2]->Movement->bActive = false;

	background->Movement->bActive = true;

	score->ResetScore();
	score->StartwithFirstPair();

	player->GameOverSprite->bActive = false;

	auto OneShotFunction = [this]()
	{
		player->Gravity->bActive = true;
		player->Movement->bActive = true;
		pipesPairs[0]->Movement->bActive = true;
		pipesPairs[1]->Movement->bActive = true;
		pipesPairs[2]->Movement->bActive = true;
		refEngine->KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.pop();
		player->Flap->Func();
		player->Animator->SpeedRate = 3.;
	};
	refEngine->KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.push(OneShotFunction);
	return 0;
}

void GameEditor::OnGameOver()
{	
	player->Flap->Func();
	refEngine->KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.push([](){});
	StopMovingObjects();
	player->Movement->bActive = true;
	auto restartFunc = [this]()
	{
		refEngine->KeyboardHandler->InputMap[SDLK_SPACE]->ActionStack.pop();
		GameStart();
		refEngine->KeyboardHandler->InputMap[SDLK_BACKSPACE]->ActionStack.pop();
	};
	refEngine->KeyboardHandler->InputMap[SDLK_BACKSPACE]->ActionStack.push(restartFunc);
}

void GameEditor::StopMovingObjects()
{
	//auto moves = ComponentManager::Get()->ComponentMap["Movable"];
	auto moves = ComponentManager::Get()->MovableContainer.components;

	for (auto entity:moves)
	{
		for (auto comp : entity.second)
			comp->bActive = false;
	}
}
