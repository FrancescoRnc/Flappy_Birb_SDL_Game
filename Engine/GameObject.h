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

	GameObject(std::string name) : ObjectName{name} { Entity = ++Entities; }

	// Position in screen coordinates.
	// These are pointers because you can use them pointing to other variables, such Rect.x/y ecc.
	int* PosX = 0;
	int* PosY = 0;

	std::string ObjectName;
	std::map<std::string, Component*> Components;

	int Entity = -1;
	static int Entities;

	template<typename T>
	auto BindComponent(std::string type, std::string name, T* component)
	{
		Components.insert(std::pair<std::string, T*>(name, component));
		ComponentManager::Get()->AddComponent(type, component);
		//component->Owner = this;
		return component;
	}
};