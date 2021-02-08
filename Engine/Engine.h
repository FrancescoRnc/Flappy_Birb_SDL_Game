#pragma once
#include <SDL.h>
#include <iostream>
#include "Scene.h"
#include "Clock.h"
#include "FileManager.h"
#include "ComponentManager.h"
#include "RenderManager.h"
#include "InputHandler.h"

//#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

struct Engine : public IInitiable, public IUpdatable
{
	WindowRectInfo WindowInfo;

	Engine(WindowRectInfo _info) : WindowInfo{_info} 
	{
		_instance = this;
		fileMgr = new FileManager();
		componentMgr = new ComponentManager();
		renderMgr = new RenderManager({"Flappy Birb", 
									  SDL_WINDOWPOS_CENTERED, 
									  SDL_WINDOWPOS_CENTERED, 
									  270, 480});
		clock = new Clock();
		current_scene = new Scene();
	};

	RULE_OF_FIVE(Engine);


	virtual int Initialize() override;
	int Loop();

	virtual int Update(const double deltatime) override;
	//void Draw();

	void OnExitGame();

	static Engine* Get()
	{
		return _instance;
	}

	FileManager* fileMgr = nullptr;
	ComponentManager* componentMgr = nullptr;
	RenderManager* renderMgr = nullptr;
	IInputHandler* InputHandler = nullptr;
	KeyMouseInputHandler* KeyboardHandler = nullptr;
	JoystickInputHandler* JoystickHandler = nullptr;

	private:
	int init_result = 0;
	Clock* clock = nullptr;
	Scene* current_scene = nullptr;

	static Engine* _instance;
};
