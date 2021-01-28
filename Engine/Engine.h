#pragma once
#include <SDL.h>
#include <iostream>
#include "System.h"
#include "Scene.h"
#include "Clock.h"
#include "FileManager.h"
#include "ComponentManager.h"
#include "InputHandler.h"


struct WindowRectInfo
{
	const char* Title;
	int ScreenX;
	int ScreenY;
	int Width;
	int Height;
};

struct Engine
{
	WindowRectInfo WindowInfo;

	Engine(WindowRectInfo _info) : WindowInfo{_info} 
	{
		fileMgr = new FileManager();
		componentMgr = new ComponentManager();
		clock = new Clock();
		current_scene = new Scene();
	};

	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;
	~Engine();


	int Init();
	int Loop();

	void Update(const float deltatime);
	void Draw();

	void OnExitGame();


	SDL_Renderer* renderer;
	FileManager* fileMgr;
	ComponentManager* componentMgr;
	IInputHandler* InputHandler;

	private:
	int init_result;
	Clock* clock;
	Scene* current_scene;
	SDL_Window* window;
};
