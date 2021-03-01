#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "Component.h"
#include "Interfaces.h"

#define UNIQUE_INSTANCE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

#define ALL_COMPONENTS SpriteComponent, AnimatorComponent, MovableComponent, RotatorComponent, GravityComponent, CollisionComponent, RelocatableComponent, FlapComponent


template<typename C>
struct ComponentContainer
{
	std::map<unsigned int, std::vector<C*>> components = {};
	std::map<unsigned int, std::vector<C*>> excluded = {};

	unsigned int ActiveQuantity = 0;

	void AddBy(int entity, C* comp) 
	{
		static_assert(std::is_base_of<Component, C>::value, "Class not derived from Component!");

		components[entity].push_back(comp);
		ActiveQuantity++;
	}

	C* GetFrom(int entity)
	{
		static_assert(std::is_base_of<Component, C>::value, "Class not derived from Component!");
		
		return components[entity][0];
	}

	~ComponentContainer() { components.clear(); }
};


class ComponentManager : public IUpdatable
{
	public:
	ComponentManager();

	UNIQUE_INSTANCE(ComponentManager);


	ComponentContainer<SpriteComponent> SpriteContainer;
	ComponentContainer<AnimatorComponent> AnimatorContainer;
	ComponentContainer<MovableComponent> MovableContainer;
	ComponentContainer<RotatorComponent> RotatorContainer;
	ComponentContainer<GravityComponent> GravityContainer;
	ComponentContainer<CollisionComponent> CollisionContainer;
	ComponentContainer<RelocatableComponent> RelocatableContainer;
	ComponentContainer<FlapComponent> FlapContainer;


	// Contains every Component instanced
	//std::map<std::string, std::vector<Component*>> ComponentMap;

	// Should be used to exclude some instanced Components from pipeline
	// NOT USED YET
	std::map<int, std::vector<Component*>> ExclusionMap;

	void AddComponent(const std::string type, Component* _component)
	{
		//ComponentMap[type].push_back(_component);
	}

	// Sorts Sprite Components by their Render Priority
	//void SortSprites();

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

