#include "ComponentManager.h"
#include <iostream>

ComponentManager* ComponentManager::_instance = nullptr;

ComponentManager::ComponentManager()
{
	ComponentMap =
	{
		{"Sprite", {}},
		{"Animator", {}},
		{"Movable", {}},
		{"Rotator", {}},
		{"Gravity", {}},
		{"Collision", {}},
		{"Relocator", {}},
		{"Flap", {}}
	};

	_instance = this;
}

ComponentManager::~ComponentManager()
{

}

void ComponentManager::SortSprites()
{
	auto sprites = ComponentMap["Sprite"];
	auto sortingLambda = [](Component* a, Component* b)
	{
		auto as = reinterpret_cast<SpriteComponent*>(a);
		auto bs = reinterpret_cast<SpriteComponent*>(b);
		return as->RenderPriority < bs->RenderPriority;
	};

	std::sort(sprites.begin(), sprites.end(), sortingLambda);

	ComponentMap["Sprite"] = sprites;
}

int ComponentManager::Update(const double deltatime)
{
	// Animations
	auto animatorComps = ComponentMap["Animator"];
	for (auto comp:animatorComps)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<AnimatorComponent*>(comp);
			c->frame_current_time += deltatime * c->SpeedRate;
			if (c->frame_current_time>=c->frame_max_time)
			{
				c->frame_current_time = 0;
				c->current_frame = (c->current_frame+1)%c->frames;
				c->rMainSprite->Texture = c->textures[c->current_frame];
			}
		}
	}

	// Gravity
	auto gravityComps = ComponentMap["Gravity"];
	for (auto comp:gravityComps)
	{
		auto c = reinterpret_cast<GravityComponent*>(comp);

		if (comp->bActive)
		{
			c->refMovable->VMoveSpeed += (c->GravityForce*deltatime);
		}
		else c->refMovable->VMoveSpeed = 0;
	}

	// Movable Objetcs
	auto movableComp = ComponentMap["Movable"];
	for (auto comp:movableComp)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<MovableComponent*>(comp);
			for (auto mov:c->Rects)
			{
				mov->x += (int)(c->HMoveSpeed*deltatime);
				mov->y += (int)(c->VMoveSpeed*deltatime);
			}
		}
	}

	// Object Relocators
	auto relocatorComps = ComponentMap["Relocator"];
	for (auto comp:relocatorComps)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<RelocatableComponent*>(comp);
			for (auto rect:c->Rects)
			{
				if (c->CheckPosition(rect))
					c->RelocateSprites(rect);
			}
		}
	}

	// Collision Handle
	auto colliders = ComponentMap["Collision"];
	for (auto comp:colliders)
	{
		if (comp->bActive)
		{
			auto c = reinterpret_cast<CollisionComponent*>(comp);
			for (auto other:colliders)
			{
				if (other->bActive&&comp!=other)
				{
					auto o = reinterpret_cast<CollisionComponent*>(other);
					if ((c->CollisionBitmask&o->CollisionPin)&&SDL_HasIntersection(c->Rect, o->Rect))
					{
						c->OnCollision(o->Rect);
						//o->OnCollision(c->Rect);
						//std::cout << "Collision between  " 
						//	<< c->Owner->ObjectName << " and " 
						//	<< o->Owner->ObjectName << std::endl;
					}
				}
			}
		}
	}

	return 0;
}
