#include "Scene.h"
#include "Engine.h"


Scene::Scene()
{

}
Scene::~Scene()
{
	for (auto go : Data.GameObjectMap)
		delete(go.second);
	Data.GameObjectMap.clear();
	
	Data.Objects.clear();
}

void Scene::Load(std::vector<IObjectPack*> objects)
{
	Data.Objects = objects;
	for (auto object : objects)
	{
		object->Load();
		Data.GameObjectMap.insert({object->MainObject->ObjectName, object->MainObject});
	}

}

void Scene::Unload()
{
	Data.Objects.clear();
	Data.GameObjectMap.clear();
}

int Scene::Update(const double deltatime)
{
	for (auto object :Data.Objects)
	{
		object->Update(deltatime);
	}
	return 0;
}
