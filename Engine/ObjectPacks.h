#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "InputHandler.h"

class IObjectPack
{
	public:

	virtual void Load() = 0;
};

class PlayerObjPack : public IObjectPack
{
	public:
	PlayerObjPack();

	GameObject* MainObject;
	SpriteComponent* Sprite;
	AnimatorComponent* Animator;
	MovableComponent* Movement;
	CollisionComponent* Collision;
	GravityComponent* Gravity;
	FlapComponent* Flap;

	// Game Over
	GameObject* GameOver;
	SpriteComponent* GameOverSprite;
	std::function<void()> OnGameOver;

	virtual void Load() override;
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

	void LocatePipes();

	bool CheckPipesX();
	
	private:
	int pipeHeight;
};


