#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "InputHandler.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "AudioSystem.h"


// Use and create these classes as true Gameplay objects. 
// These contain A Gameobject (one or more), some components,
// and other objects and behaviours for your Gameplay Design

class PlayerObjPack : public IObjectPack, public ILocatable
{
	public:
	PlayerObjPack();

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

	virtual void Load(ComponentManager* mgr) override;

	virtual int Update(const double deltatime) override;

	// Inherited via ILocatable
	virtual void Locate(int x, int y) override;
};

class BackgroundObjPack : public IObjectPack
{
	public:
	BackgroundObjPack();

	SpriteComponent* BGSprite;
	SpriteComponent* BaseSprite;
	MovableComponent* Movement;
	CollisionComponent* Collision;
	RelocatableComponent* BGRelocator;

	virtual void Load(ComponentManager* mgr) override;

	virtual int Update(const double deltatime) override;
};

class PipesPairObjPack : public IObjectPack, public ILocatable
{
	public:
	PipesPairObjPack();
	PipesPairObjPack(int x, int y);

	SpriteComponent* TopSprite;
	SpriteComponent* BottomSprite;
	MovableComponent* Movement;
	CollisionComponent* TopCollision;
	CollisionComponent* BottomCollision;
	RelocatableComponent* Relocator;

	virtual void Load(ComponentManager* mgr) override;

	virtual int Update(const double deltatime) override;

	void LocatePipes(int newX);
	void LocatePipesAdditive(int addX);
	void RandomizeY();
	
	// Inherited via ILocatable
	virtual void Locate(int x, int y) override;

	private:
	int pipeHeight;

};

struct ScoreBridge;

class ScoreObjPack : public IObjectPack
{
	public:

	ScoreObjPack(std::vector<PipesPairObjPack*> pairs);

	ScoreBridge* Bridge = nullptr;
	std::vector<PipesPairObjPack*> Pairs;
	PipesPairObjPack* CurrentPipesPair = nullptr;
	int currentPipesIndex = 0;
	int ScorePoints;
	SDL_Rect Hitbox;

	CollisionComponent* ScoreCollision = nullptr;

	// Inherited via IObjectPack
	virtual void Load(ComponentManager* mgr) override;

	virtual int Update(const double deltatime) override;
	// - - - -

	void StartwithFirstPair();
	PipesPairObjPack* GetNextPair();
	void IncreaseScore();
	void ResetScore();
};

class ScoreCounterObjPack : public IObjectPack
{
	public:

	ScoreCounterObjPack();

	ScoreBridge* Bridge = nullptr;
	std::vector<SDL_Texture*> NumbersTex = {};

	SpriteComponent* SpriteUnit = nullptr;
	SpriteComponent* SpriteTen = nullptr;
	SpriteComponent* SpriteHundred = nullptr;

	// Inherited via IObjectPack
	virtual void Load(ComponentManager* mgr) override;

	virtual int Update(const double deltatime) override;
	// - - - -

	void ChangeNumber();
};


struct ScoreBridge
{
	ScoreBridge(ScoreObjPack* score, ScoreCounterObjPack* counter) :
	refScore{score}, refCounter{counter} 
	{
		refScore->Bridge = this;
		refCounter->Bridge = this;
	}

	ScoreObjPack* refScore;
	ScoreCounterObjPack* refCounter;
};