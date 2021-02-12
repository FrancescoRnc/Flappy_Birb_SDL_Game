#include "RenderManager.h"

RenderManager* RenderManager::_instance = nullptr;

RenderManager::RenderManager(WindowRectInfo _info)
{
	WindowInfo = _info;
	
	SpriteBuffer = {
		{RenderLayer::Background, {}},
		{RenderLayer::Environment, {}},
		{RenderLayer::BackgroundBase, {}},
		{RenderLayer::Player, {}},
		{RenderLayer::HUD, {}}
	};

	_instance = this;
}

RenderManager::~RenderManager()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
}

int RenderManager::Initialize()
{
	std::cout << "Creating Window..." << std::endl;
	Window = SDL_CreateWindow(WindowInfo.Title,
							  WindowInfo.ScreenX,
							  WindowInfo.ScreenY,
							  WindowInfo.Width,
							  WindowInfo.Height,
							  SDL_WINDOW_SHOWN);
	if (!Window)
	{
		std::cout << "Error during Window Creation" << std::endl;
		return 1;
	}
	std::cout << "Window Created Succesfully!" << std::endl << std::endl;

	std::cout << "Creating Renderer..." << std::endl;
	Renderer = SDL_CreateRenderer(Window, -1,
								  SDL_RENDERER_PRESENTVSYNC |
								  SDL_RENDERER_ACCELERATED);
	if (!Renderer)
	{
		std::cout << "Error during Renderer Creation" << std::endl;
		return 1;
	}
	std::cout << "Renderer Created Succesfully!" << std::endl << std::endl;

	return 0;
}

int RenderManager::Draw()
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Renderer);

	for (auto layer:SpriteBuffer)
	{
		for (auto comp : layer.second)
		{
			if (comp->bActive)
				if (SDL_RenderCopyEx(Renderer,
									 comp->Texture,
									 &comp->SrcRect,
									 &comp->DstRect,
									 *comp->RotationAngleDegree,
									 comp->Point,
									 comp->FlipRule))
				{
					return 1;
				}
		}
	}

	SDL_RenderPresent(Renderer);

	return 0;
}

void RenderManager::RegisterSprite(RenderLayer layer, SpriteComponent* sprite)
{
	SpriteBuffer[layer].push_back(sprite);
}
