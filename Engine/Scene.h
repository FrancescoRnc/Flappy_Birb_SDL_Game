#pragma once
#include <map>
#include <string>
#include "GameObject.h"
#include "InputHandler.h"
#include "ObjectPacks.h"

struct Scene
{
	Scene();
	~Scene();

	void LoadObjects();
	void UpdateComponents(const double deltatime, std::map<std::string, std::vector<Component*>> cMap);
	void DrawComponents(SDL_Renderer* renderer, std::vector<Component*> sprites);

	private:

	PlayerObjPack* Player = nullptr;
	PipesPairObjPack* Pipes = nullptr;
	BackgroundObjPack* Background = nullptr;

	std::map<std::string, GameObject*> GameObjectMap;

	void StopMovingObjets(std::vector<MovableComponent*> components);

	//std::map<GameObject*, Component*> ComponentsBinded;
};

