#pragma once
#include <SDL.h>
#include <vector>
#include <map>
#include "ComponentManager.h"
#include "FileManager.h"

class GameObject
{
	public:
	GameObject();
	~GameObject();

	GameObject(std::string name) : ObjectName{name}
	{
		PosX = 0;
		PosY = 0;
	}

	std::string ObjectName;

	int* PosX;
	int* PosY;

	std::map<std::string, Component*> Components;

	template<typename T>
	auto BindComponent(std::string type, std::string name, T* component) -> Component*
	{
		Components.insert(std::pair<std::string, T*>(name, component));
		ComponentManager::Get()->AddComponent(type, component);
		component->Owner = this;
		return component;
	}
};

