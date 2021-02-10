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
	PipesPairObjPack(int x, int y);

	GameObject* MainObject;
	SpriteComponent* TopSprite;
	SpriteComponent* BottomSprite;
	MovableComponent* Movement;
	CollisionComponent* TopCollision;
	CollisionComponent* BottomCollision;
	RelocatableComponent* Relocator;

	virtual void Load() override;

	virtual int Update(const double deltatime) override;

	void LocatePipes(int newX);
	void LocatePipesAdditive(int addX);
	void RandomizeY();
	
	private:
	int pipeHeight;
};

struct ScoreBridge;

class ScoreObjPack : public IObjectPack
{
	public:

	ScoreObjPack(std::vector<PipesPairObjPack*> pairs);

	GameObject* MainObject = nullptr;
	ScoreBridge* Bridge = nullptr;
	std::vector<PipesPairObjPack*> Pairs;
	PipesPairObjPack* CurrentPipesPair = nullptr;
	int currentPipesIndex = 0;
	int ScorePoints;
	SDL_Rect Hitbox;

	CollisionComponent* ScoreCollision = nullptr;

	// Inherited via IObjectPack
	virtual void Load() override;

	virtual int Update(const double deltatime) override;
	// - - - -

	PipesPairObjPack* GetNextPair();
	void IncreaseScore();
};

class ScoreCounterObjPack : public IObjectPack
{
	public:

	ScoreCounterObjPack();

	GameObject* MainObject;
	ScoreBridge* Bridge = nullptr;
	std::vector<SDL_Texture*> NumbersTex = {};

	SpriteComponent* SpriteUnit = nullptr;
	SpriteComponent* SpriteTen = nullptr;
	SpriteComponent* SpriteHundred = nullptr;

	// Inherited via IObjectPack
	virtual void Load() override;

	virtual int Update(const double deltatime) override;
	// - - - -

	void ChangeNumber();
};


struct ScoreBridge
{
	ScoreObjPack* refScore;
	ScoreCounterObjPack* refCounter;
};