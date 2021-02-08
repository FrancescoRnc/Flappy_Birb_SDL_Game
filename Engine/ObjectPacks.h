#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "InputHandler.h"
#include "Interfaces.h"
#include "RenderManager.h"

/*class IObjectPack
{
	public:

	virtual void Load() = 0;

	virtual void Update(const double deltatime) = 0;
};*/

class PlayerObjPack : public IObjectPack
{
	public:
	PlayerObjPack();

	GameObject* MainObject;
	SpriteComponent* Sprite;
	AnimatorComponent* Animator;
	MovableComponent* Movement;
	RotatorComponent* Rotator;
	CollisionComponent* Collision;
	GravityComponent* Gravity;
	FlapComponent* Flap;

	// Game Over
	GameObject* GameOver;
	SpriteComponent* GameOverSprite;
	std::function<void()> OnGameOver;

	virtual void Load() override;

	virtual int Update(const double deltatime) override;
};

class BackgroundObjPack : public IObjectPack
{
	public:
	BackgroundObjPack();

	GameObject* MainObject;
	SpriteComponent* BGSprite;
	SpriteComponent* BaseSprite;
	MovableComponent* Movement;
	CollisionComponent* Collision;
	RelocatableComponent* BGRelocator;

	virtual void Load() override;

	virtual int Update(const double deltatime) override;
};

class PipesPairObjPack : public IObjectPack
{
	public:
	PipesPairObjPack();

	GameObject* MainObject;
	SpriteComponent* TopSprite;
	SpriteComponent* BottomSprite;
	MovableComponent* Movement;
	CollisionComponent* TopCollision;
	CollisionComponent* BottomCollision;
	RelocatableComponent* Relocator;

	virtual void Load() override;

	virtual int Update(const double deltatime) override;

	void LocatePipes();
	
	private:
	int pipeHeight;
};


