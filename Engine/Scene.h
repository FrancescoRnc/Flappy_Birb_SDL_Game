#pragma once
#include <map>
#include <string>
#include "GameObject.h"
#include "InputHandler.h"
#include "ObjectPacks.h"


struct SceneData
{
	std::vector<IObjectPack*> Objects = {};
	std::map<std::string, GameObject*> GameObjectMap = {};
};


struct Scene : public IScene
{
	Scene();
	~Scene();

	//void LoadObjects();
	//void UpdateComponents(const double deltatime, std::map<std::string, std::vector<Component*>> cMap);


	std::map<std::string, GameObject*> GameObjectMap;

	SceneData Data = {};

	void StopMovingObjets(std::vector<MovableComponent*> components);


	// Inherited via IScene
	virtual void Load(std::vector<IObjectPack*> objects) override;

	virtual void Unload() override;

	virtual int Update(const double deltatime) override;
	// - - - -

	//std::map<GameObject*, Component*> BindedComponents;
};



struct GameState
{
	GameState();

	GameState* Next;

	void ChangeState();
};

