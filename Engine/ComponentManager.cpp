#include "ComponentManager.h"
#include <iostream>

ComponentManager* ComponentManager::_instance = nullptr;

ComponentManager::ComponentManager()
{
	ComponentMap =
	{
		{"Sprite", {}},
		{"Animator", {}},
		{"Movable", {}},
		{"Rotator", {}},
		{"Gravity", {}},
		{"Collision", {}},
		{"Relocator", {}},
		{"Flap", {}}
	};

	_instance = this;
}

ComponentManager::~ComponentManager()
{

}

void ComponentManager::SortSprites()
{
	auto sprites = ComponentMap["Sprite"];

	//std::cout << "Rendering Order before: " << std::endl;
	//for (auto comp : sprites)
	//{
	//	auto c = reinterpret_cast<SpriteComponent*>(comp);
	//	std::cout << c->RenderPriority << std::endl;
	//
	//}

	auto sortingLambda = [](Component* a, Component* b)
	{
		auto as = reinterpret_cast<SpriteComponent*>(a);
		auto bs = reinterpret_cast<SpriteComponent*>(b);
		return as->RenderPriority < bs->RenderPriority;
	};
	std::sort(sprites.begin(), sprites.end(), sortingLambda);

	//std::cout << "Rendering Order after: " << std::endl;
	//for (auto comp : sprites)
	//{
	//	auto c = reinterpret_cast<SpriteComponent*>(comp);
	//	std::cout << c->RenderPriority << std::endl;
	//
	//}

	ComponentMap["Sprite"] = sprites;
}
