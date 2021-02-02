#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include "Component.h"

struct WindowRectInfo
{
	const char* Title;
	int ScreenX;
	int ScreenY;
	int Width;
	int Height;
};

class RenderManager
{
	public:
	RenderManager(WindowRectInfo _info);

	RenderManager(const RenderManager&) = delete;
	RenderManager(RenderManager&&) = delete;
	RenderManager operator= (const RenderManager&) = delete;
	RenderManager operator= (RenderManager&&) = delete;

	~RenderManager();

	SDL_Window* Window;
	SDL_Renderer* Renderer;
	WindowRectInfo WindowInfo;

	std::vector<SpriteComponent*> SpriteComponents;
	//std::vector<AnimatorComponent*> AnimatorComponents;

	int Init();
	int Draw();

	static RenderManager* Get()
	{
		return _instance;
	}

	private:
	static RenderManager* _instance;
};

