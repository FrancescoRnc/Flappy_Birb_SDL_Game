#pragma once
#include <map>
#include <string>
#include "GameObject.h"
#include "InputHandler.h"
#include "ObjectPacks.h"

// This struct contains data that the scene uses for itself
struct SceneData
{
	std::vector<IObjectPack*> Objects = {};
	std::map<std::string, GameObject*> GameObjectMap = {};
};


struct Scene : public IScene
{
	Scene();

	RULE_OF_FIVE(Scene);

	SceneData Data = {};

	// Inherited via IScene
	virtual void Load(std::vector<IObjectPack*> objects) override;

	virtual void Unload() override;

	virtual int Update(const double deltatime) override;
	// - - - -
};

