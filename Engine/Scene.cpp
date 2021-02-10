#include "Scene.h"
#include "Engine.h"


Scene::Scene()
{

}
Scene::~Scene()
{

}

/*void Scene::LoadObjects()
{
	// Loading Gameobjects...

    Player = new PlayerObjPack();
    Player->Load();
	Pipes = new PipesPairObjPack();
	Pipes->Load();
	Background = new BackgroundObjPack();
	Background->Load();

	ComponentManager::Get()->SortSprites();

	GameObjectMap.insert({Background->MainObject->ObjectName, Background->MainObject});
	GameObjectMap.insert({Pipes->MainObject->ObjectName, Pipes->MainObject});
	GameObjectMap.insert({Player->MainObject->ObjectName, Player->MainObject});
	GameObjectMap.insert({Player->GameOver->ObjectName, Player->GameOver});
}
void Scene::UpdateComponents(const double deltatime, std::map<std::string, std::vector<Component*>> cMap)
{
	// Gravity
	auto gravityComps = cMap["Gravity"];
	for (auto comp : gravityComps)
	{
		auto c = reinterpret_cast<GravityComponent*>(comp);
		
		if (comp->bActive)
		{
			c->refMovable->VMoveSpeed += (c->GravityForce * deltatime);
		}
		else c->refMovable->VMoveSpeed = 0;
	}

	// refMovable Objetcs
	auto movableComp = cMap["Movable"];
	for (auto comp : movableComp)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<MovableComponent*>(comp);
			for (auto mov : c->Rects)
			{
				mov->x += (int)(c->HMoveSpeed * deltatime);
				mov->y += (int)(c->VMoveSpeed * deltatime);
			}
		}
	}

	auto relocatorComps = cMap["Relocator"];
	for (auto comp : relocatorComps)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<RelocatableComponent*>(comp);
			for (auto rect : c->Rects)
			{
				if (c->CheckPosition(rect))
					c->RelocateSprites(rect);
			}
		}
	}

	// Collision Handle
	auto colliders = cMap["Collision"];
	for (auto comp : colliders)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<CollisionComponent*>(comp);
			for (auto other : colliders)
			{
				if (other->bActive && comp != other)
				{
					auto o = reinterpret_cast<CollisionComponent*>(other);
					if ((c->CollisionBitmask & o->CollisionPin) && SDL_HasIntersection(c->Rect, o->Rect))
					{
						c->OnCollision(o->Rect);
						o->OnCollision(c->Rect);
						std::cout << "Collision between " << c->Owner->ObjectName << " and " << o->Owner->ObjectName << std::endl;
					}
				}
			}
		}
	}

	// Animations
	auto animatorComps = cMap["Animator"];
	for (auto comp : animatorComps)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<AnimatorComponent*>(comp);
			c->frame_current_time += deltatime;
			//std::cout << "Time: " << c->frame_current_time << std::endl;
			if (c->frame_current_time >= c->frame_max_time)
			{
				c->frame_current_time = 0;
				c->current_frame = (c->current_frame+1) % c->frames;
				c->rMainSprite->Texture = c->textures[c->current_frame];
				//std::cout << "Current Frame: " << c->current_frame << std::endl;
			}
		}
	}

	//Player->Update(deltatime);
	Update(deltatime);
}*/

void Scene::StopMovingObjets(std::vector<MovableComponent*> components)
{
	for (auto comp : components)
	{
		comp->bActive = false;
	}
}

void Scene::Load(std::vector<IObjectPack*> objects)
{
	Data.Objects = objects;
	for (auto object : objects)
	{
		object->Load();
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
