#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include "Component.h"
#include "Interfaces.h"

#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

// Enum used to sort Sprite Components by a certain Layer
enum class RenderLayer { Background = 1, 
						 Environment = 5, 
	                     BackgroundBase = 10,
						 Player = 20, 
						 HUD = 100
};

// This struct contains and uses info for your window
struct WindowRectInfo
{
	const char* Title;
	int Width;
	int Height;
	int ScreenX;
	int ScreenY;
};

// These class handles every Render Object and behaviour for itself
class RenderManager : public IInitiable
{
	public:
	RenderManager(WindowRectInfo _info);

	RULE_OF_FIVE(RenderManager);

	SDL_Window* Window;
	SDL_Renderer* Renderer;
	WindowRectInfo WindowInfo;

	// Map that contains every Sprite Component, sorted by Render Layers
	std::map<RenderLayer, std::vector<SpriteComponent*>> SpriteBuffer;

	virtual int Initialize() override;

	// The true Draw function, that renders every Active Texture
	int Draw();

	void RegisterSprite(RenderLayer layer, SpriteComponent* sprite);

	static RenderManager* Get()
	{
		return _instance;
	}

	private:
	static RenderManager* _instance;
};

