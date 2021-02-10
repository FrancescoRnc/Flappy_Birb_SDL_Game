#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "Component.h"
#include "Interfaces.h"

//#define std::tuple<SpriteComponent, AnimatorComponent, MovableComponent, GravityComponent> Comps
#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();


class ComponentManager : public IUpdatable
{
	public:
	ComponentManager();

	RULE_OF_FIVE(ComponentManager);

	std::map<std::string, std::vector<Component*>> ComponentMap;
	std::map<std::string, std::vector<Component*>> ComponentExclusionMap;

	//Components ComponentDatabase;

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

	// Inherited via IUpdatable
	virtual int Update(const double deltatime) override;
	// - - - -

	private:
	static ComponentManager* _instance;

	
};

