#include "Scene.h"
#include "Engine.h"


Scene::Scene()
{

}
Scene::~Scene()
{

}

void Scene::LoadObjects()
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
	
	Pipes->LocatePipes();
}
void Scene::UpdateComponents(const float deltatime, std::map<std::string, std::vector<Component*>> cMap)
{
	// Gravity
	auto gravityComps = cMap["Gravity"];
	for (auto comp : gravityComps)
	{
		auto c = reinterpret_cast<GravityComponent*>(comp);
		
		if (comp->bActive)
		{
			c->Movable->VMoveSpeed += (c->GravityForce * deltatime);
		}
		else c->Movable->VMoveSpeed = 0;
	}

	// Movable Objetcs
	auto movableComp = cMap["Movable"];
	for (auto comp : movableComp)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<MovableComponent*>(comp);
			for (auto mov : c->Rects)
			{
				mov->x += (c->HMoveSpeed * deltatime);
				mov->y += (c->VMoveSpeed * deltatime);
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
				if (other->bActive)
				{
					auto o = reinterpret_cast<CollisionComponent*>(other);
					if ((c->CollisionPin & o->CollisionBitmask) && SDL_HasIntersection(c->Rect, o->Rect))
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
}
void Scene::DrawComponents(SDL_Renderer* renderer, std::vector<Component*> sprites)
{
	auto spriteComps = ComponentManager::Get()->ComponentMap["Sprite"];
	for (auto comp : spriteComps)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<SpriteComponent*>(comp);
			SDL_RenderCopyEx(renderer, c->Texture, &c->SrcRect, &c->DstRect, 0, c->Point, c->FlipRule);
		}
	}
}

void Scene::StopMovingObjets(std::vector<MovableComponent*> components)
{
	for (auto comp : components)
	{
		comp->bActive = false;
	}
}
