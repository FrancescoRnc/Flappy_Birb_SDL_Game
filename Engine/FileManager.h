#pragma once

#include <SDL.h>
#include "stb_image.h"
#include <vector>
#include <map>
#include <filesystem>

//#define CHECK(I) if (I) return 1;
#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

class FileManager
{
	public:
	FileManager();

	RULE_OF_FIVE(FileManager);

	static FileManager* Get()
	{
		return _instance;
	}

	int LoadTextures(std::string path, SDL_Renderer* _renderer);
	int LoadAudio(std::string path);

	SDL_Texture* GetTexture(std::string name);

	private:
	static FileManager* _instance;

	std::map<std::string, SDL_Texture*> Textures;


};

