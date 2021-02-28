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
class ComponentContainer
{
	public:

	std::map<int, std::vector<C*>> components = {};

	void AddBy(int entity, C* comp) 
	{
		static_assert(std::is_base_of<Component, C>::value, "Class not derived from Component!");

		components[entity].push_back(comp);
	}

	C* GetFrom(int entity)
	{
		static_assert(std::is_base_of<Component, C>::value, "Class not derived from Component!");
		
		return components[entity][0];
	}
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
	std::map<std::string, std::vector<Component*>> ComponentExclusionMap;

	void AddComponent(const std::string type, Component* _component)
	{
		//std::tuple<SpriteComponent*, AnimatorComponent*> comps = {new SpriteComponent(), new AnimatorComponent()}; 
		//ComponentMap[type].push_back(_component);
	}

	template<typename ... Args>
	auto CreateEmptyMap()
	{
		std::map<std::string, std::vector<Component*>> map;

		//map.insert({typeid(Args).name(), {}});
		
		return map;
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

