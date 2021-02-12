#include "FileManager.h"
#include <iostream>

FileManager* FileManager::_instance = nullptr;

FileManager::FileManager()
{
	Textures = {};
	_instance = this;
}

FileManager::~FileManager()
{
	
	Textures.clear();
}


int FileManager::LoadTextures(const std::string path, SDL_Renderer* _renderer)
{
	SDL_RendererInfo info;
	SDL_GetRendererInfo(_renderer, &info);
	auto files = std::filesystem::directory_iterator(path);
	for (auto file : files)
	{
		// Finding file path and a name to use for Map
		// Ex: "gameover.png" -> name = "gameover"
		std::cout << file.path() << std::endl;
		auto name = file.path().string().substr(path.size());
		auto trueNameSize = name.size() - 4;
		name = name.substr(0, trueNameSize);
		
		int width, height, comp;
		auto image = stbi_load(file.path().string().c_str(),
							   &width, &height, &comp, 4);
		if (!image) return 1;

		auto texture = SDL_CreateTexture(_renderer, 
										 SDL_PIXELFORMAT_ABGR8888,
										 SDL_TEXTUREACCESS_STREAMING,
										 width, height);
		if (!texture) return 1;

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		
		Uint8* pixels = nullptr;
		int pitch = 0;
		if (SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch))
			return 1;
		SDL_memcpy(pixels, image, pitch * height);
		delete(image);
		SDL_UnlockTexture(texture);
		Textures.insert({name, texture});
	}
	std::cout << "Images loaded succesfully!" << std::endl;
	std::cout << std::endl;

	return 0;
}

SDL_Texture* FileManager::GetTexture(const std::string name)
{
	return Textures[name];
}