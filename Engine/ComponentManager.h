#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "Component.h"
#include "Interfaces.h"

#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

class ComponentManager : public IUpdatable
{
	public:
	ComponentManager();

	RULE_OF_FIVE(ComponentManager);

	// Contains every Component instanced
	std::map<std::string, std::vector<Component*>> ComponentMap;

	// Should be used to exclude some instanced Components from pipeline
	// NOT USED YET
	std::map<std::string, std::vector<Component*>> ComponentExclusionMap;

	void AddComponent(const std::string type, Component* _component)
	{
		ComponentMap[type].push_back(_component);
	}

	// Sorts Sprite Components by their Render Priority
	void SortSprites();

	// Inherited via IUpdatable
	virtual int Update(const double deltatime) override;
	// - - - -

	static ComponentManager* Get()
	{
		return _instance;
	}

	private:
	static ComponentManager* _instance;

	
};

