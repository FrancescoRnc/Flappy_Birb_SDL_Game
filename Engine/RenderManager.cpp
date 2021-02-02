#include "RenderManager.h"

RenderManager::RenderManager(WindowRectInfo _info)
{
	WindowInfo = _info;
	
}

RenderManager::~RenderManager()
{

}

int RenderManager::Init()
{
	std::cout << "Initialization..." << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO | 0))
	{
		std::cout << "Error during Initialization" << std::endl;
		return 1;
	}

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
	for (auto comp : SpriteComponents)
	{
		if (SDL_RenderCopyEx(Renderer,
							 comp->Texture,
							 &comp->SrcRect,
							 &comp->DstRect,
							 *comp->RotationAngle,
							 comp->Point,
							 comp->FlipRule))
			return 1;
	}
	return 0;
}
