#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "Component.h"

#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

class ComponentManager
{
	public:
	ComponentManager();

	RULE_OF_FIVE(ComponentManager);

	std::map<std::string, std::vector<Component*>> ComponentMap;
	std::map<std::string, std::vector<Component*>> ComponentExclusionMap;

	static ComponentManager* Get()
	{
		return _instance;
	}

	template<typename T>
	auto GetComponentByOwner(std::string type, GameObject* owner)
	{
		return [owner, ComponentMap]()
		{
			for (auto comp : ComponentMap[type])
				if (comp->Owner == owner) return comp->Owner;
		};
	}

	void AddComponent(std::string type, Component* _component)
	{
		ComponentMap[type].push_back(_component);
	}

	void SortSprites();
	// Specialized AddComponent
	/*template<typename T>
	void AddComponent(T* _comp)
	{		
		//BaseComponents.push_back(_comp);
	}

	template<>
	void AddComponent<SpriteComponent>(SpriteComponent* _comp)
	{
		SpriteComponents.push_back(_comp);
	}
	
	template<>
	void AddComponent<AnimatorComponent>(AnimatorComponent* _comp)
	{
		AnimatorComponents.push_back(_comp);
	}

	template<>
	void AddComponent<MovableComponent>(MovableComponent* _comp)
	{
		MovableComponents.push_back(_comp);
	}

	template<>
	void AddComponent<GravityComponent>(GravityComponent* _comp)
	{
		GravityComponents.push_back(_comp);
	}*/

	private:
	static ComponentManager* _instance;
};

