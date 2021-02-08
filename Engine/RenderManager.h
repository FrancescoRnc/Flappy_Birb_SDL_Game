#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include "Component.h"
#include "Interfaces.h"

#define BACKGROUND_LAYER 0;
#define CUSTOM01_LAYER 1;
#define CUSTOM02_LAYER 2;

#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

enum class RenderLayer { Background = 1, 
						 Environment = 5, 
	                     BackgroundBase = 10,
						 Player = 20, 
						 HUD = 100
};

struct WindowRectInfo
{
	const char* Title;
	int ScreenX;
	int ScreenY;
	int Width;
	int Height;
};

class RenderManager : public IInitiable
{
	public:
	RenderManager(WindowRectInfo _info);

	RULE_OF_FIVE(RenderManager);
	//RenderManager(const RenderManager&) = delete;
	//RenderManager(RenderManager&&) = delete;
	//RenderManager operator= (const RenderManager&) = delete;
	//RenderManager operator= (RenderManager&&) = delete;

	//~RenderManager();

	SDL_Window* Window;
	SDL_Renderer* Renderer;
	WindowRectInfo WindowInfo;

	std::map<RenderLayer, std::vector<SpriteComponent*>> SpriteBuffer;

	virtual int Initialize() override;
	int Draw();

	void RegisterSprite(RenderLayer layer, SpriteComponent* sprite);

	static RenderManager* Get()
	{
		return _instance;
	}

	private:
	static RenderManager* _instance;
};

