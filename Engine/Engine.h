#pragma once
#include <SDL.h>
#include <iostream>
#include "Scene.h"
#include "Clock.h"
#include "FileManager.h"
#include "AudioSystem.h"
#include "ComponentManager.h"
#include "RenderManager.h"
#include "InputHandler.h"
#include "GameEditor.h"


struct Engine : public IInitiable, public IUpdatable
{
	Engine();

	RULE_OF_FIVE(Engine);

	virtual int Initialize() override;

	// This function is used for the main Engine Loop pipeline
	int Loop();

	// This function updates scene and managers' steps
	virtual int Update(const double deltatime) override;

	void OnStartGame();

	// This function is called before the program ends
	void OnExitGame();

	static Engine* Get()
	{
		return _instance;
	}

	FileManager* fileMgr = nullptr;
	AudioSystem* audioSys = nullptr;
	ComponentManager* componentMgr = nullptr;
	RenderManager* renderMgr = nullptr;
	KeyMouseInputHandler* KeyboardHandler = nullptr;
	GameEditor* gameEditor = nullptr;

	private:
	Clock* clock = nullptr;
	Scene* current_scene = nullptr;

	static Engine* _instance;
};
