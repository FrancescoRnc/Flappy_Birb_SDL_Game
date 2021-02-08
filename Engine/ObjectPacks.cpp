#include "ObjectPacks.h"
#include <cmath>


PlayerObjPack::PlayerObjPack()
{
	MainObject = new GameObject("Player");
	Animator = new AnimatorComponent({FileManager::Get()->
									  GetTexture("redbird-upflap"),
									  FileManager::Get()->
									  GetTexture("redbird-midflap"),
									  FileManager::Get()->
									  GetTexture("redbird-downflap"),
									  FileManager::Get()->
									  GetTexture("redbird-midflap")});
	Sprite = new SpriteComponent(nullptr, Animator->textures[0],
								 {}, {70, 200}, nullptr, SDL_FLIP_NONE);
	Movement = new MovableComponent();
	Rotator = new RotatorComponent();
	Gravity = new GravityComponent();
	Collision = new CollisionComponent();
	Flap = new FlapComponent();
	Flap->DoFlap = new InputAction();

	GameOver = new GameObject("Game Over");
	GameOverSprite = new SpriteComponent(nullptr, FileManager::Get()->
										 GetTexture("gameover"),
										 {}, {39, 100}, nullptr, SDL_FLIP_NONE);

}

void PlayerObjPack::Load()
{
	MainObject->PosX = &Sprite->DstRect.x;
	MainObject->PosY = &Sprite->DstRect.y;
	Sprite->RenderPriority = 20;
	Animator->rMainSprite = Sprite;
	Animator->bLoop = true;
	Animator->frame_max_time = 1. / 15.;
	Movement->Rects = {&Sprite->DstRect};
	Rotator->MinAngle = -90; Rotator->MaxAngle = 90.;
	Sprite->RotationAngleDegree = Rotator->Rotation;
	Gravity->GravityForce = (98.1 * 10.);
	Gravity->refMovable = Movement;
	Gravity->bActive = false;

	Collision->Rect = &Sprite->DstRect;
	Collision->CollisionPin = (uint8_t)0x00000001;
	Collision->CollisionBitmask = (uint8_t)0x00000110;
	Collision->OnCollision = [this](SDL_Rect* other)
	{
		OnGameOver();
		Flap->DoFlap->Action();
		GameOverSprite->bActive = true;
		Collision->bActive = false;
		Flap->bActive = false;
		std::cout << "Player Collision" << std::endl;
	};

	Flap->Gravity = Gravity; 
	Flap->FlapForce = 300.;
	Flap->KeyCode = SDLK_SPACE;
	Flap->DoFlap->Action = [this]()
	{
		Gravity->bActive = true;
		Movement->VMoveSpeed = -Flap->FlapForce;
		//std::cout << "Flap Flap!" << std::endl;
	};

	GameOverSprite->bActive = false; GameOverSprite->RenderPriority = 100;
	OnGameOver = [this]()
	{
		auto components = ComponentManager::Get()->ComponentMap["Movable"];
		for (auto c : components) c->bActive = false;
		Movement->bActive = true;
	};

	MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite", Sprite);
	auto rm = RenderManager::Get();
	rm->RegisterSprite(RenderLayer::Player, Sprite);
	MainObject->BindComponent<AnimatorComponent>("Animator", "Flap Animation", Animator);
	MainObject->BindComponent<MovableComponent>("Movable", "Movement", Movement);
	MainObject->BindComponent<RotatorComponent>("Rotator", "Rotator", Rotator);
	MainObject->BindComponent<CollisionComponent>("Collision", "Collider", Collision);
	MainObject->BindComponent<GravityComponent>("Gravity", "Gravity",  Gravity);
	MainObject->BindComponent<FlapComponent>("Flap", "Flap",  Flap);
	GameOver->  BindComponent<SpriteComponent>("Sprite", "Game Over Sprite", GameOverSprite);
	rm->RegisterSprite(RenderLayer::Player, GameOverSprite);
}

int PlayerObjPack::Update(const double deltatime)
{
	if (Movement->VMoveSpeed > 0)
	{
		if (*Rotator->Rotation <= Rotator->MaxAngle)
			*Rotator->Rotation += 180. * deltatime;
	}
	else if (Movement->VMoveSpeed == 0) *Rotator->Rotation = 0;
	else *Rotator->Rotation = -15.;

	if (*MainObject->PosY >= 700)
	{
		Gravity->bActive = false;
		Movement->VMoveSpeed = 0;
		Movement->bActive = false;
	}
	//*Rotator->Rotation = sin(Movement->VMoveSpeed) * (180.f / M_PI);
	//std::cout << "Rotation: " << *Rotator->Rotation << std::endl;
	return 0;
}

BackgroundObjPack::BackgroundObjPack()
{
	MainObject = new GameObject("Background");
	BGSprite = new SpriteComponent(nullptr, FileManager::Get()->
								   GetTexture("background-day-extend"),
								   {}, {}, nullptr, SDL_FLIP_NONE);
	BaseSprite = new SpriteComponent(nullptr, FileManager::Get()->
									 GetTexture("base-extend"),
									 {}, {0, 400}, nullptr, SDL_FLIP_NONE);
	Movement = new MovableComponent();
	Collision = new CollisionComponent();
	BGRelocator = new RelocatableComponent();
}

void BackgroundObjPack::Load()
{
	BGSprite->RenderPriority = 1;
	BaseSprite->RenderPriority = 10;
	Movement->Rects = {
		&BGSprite->DstRect, 
		&BaseSprite->DstRect, 
	};
	Movement->HMoveSpeed = -60.;
	Collision->Rect = &BaseSprite->DstRect;
	Collision->CollisionPin = (uint8_t)0x00000100;
	Collision->CollisionBitmask = (uint8_t)0x00000001;
	Collision->OnCollision = [this](SDL_Rect* other)
	{
		std::cout << "Base Collision" << std::endl;
	};
	BGRelocator->Rects = {
		&BGSprite->DstRect,
		&BaseSprite->DstRect,
	};
	BGRelocator->LimitX = -(BGSprite->DstRect.w / 2);
	BGRelocator->CheckPosition = [this](SDL_Rect* outRect)
	{ 
		return outRect->x < BGRelocator->LimitX; 
	};
	BGRelocator->RelocateSprites = [this](SDL_Rect* inRect)
	{
		inRect->x = 0;
	};


	MainObject->BindComponent<SpriteComponent>("Sprite", "Background Sprite", BGSprite);
	MainObject->BindComponent<SpriteComponent>("Sprite", "Base Sprite", BaseSprite);
	auto rm = RenderManager::Get();
	rm->RegisterSprite(RenderLayer::Background, BGSprite);
	rm->RegisterSprite(RenderLayer::BackgroundBase, BaseSprite);
	MainObject->BindComponent<MovableComponent>("Movable", "Background Movement", Movement);
	MainObject->BindComponent<CollisionComponent>("Collision", "Base Collider", Collision);
	MainObject->BindComponent<RelocatableComponent>("Relocator", "Relocator", BGRelocator);
}

int BackgroundObjPack::Update(const double deltatime)
{
	return 0;
}

PipesPairObjPack::PipesPairObjPack()
{
	MainObject = new GameObject("Pipes");

	TopSprite = new SpriteComponent(nullptr, FileManager::Get()->
									GetTexture("pipe-green"),
									{}, {}, nullptr, SDL_FLIP_VERTICAL);
	TopCollision = new CollisionComponent();
	
	BottomSprite = new SpriteComponent();
	BottomCollision = new CollisionComponent();
	
	Movement = new MovableComponent();
	Relocator = new RelocatableComponent();

	SDL_QueryTexture(TopSprite->Texture, nullptr, nullptr, nullptr, &pipeHeight);
}

void PipesPairObjPack::Load()
{
	TopSprite->RenderPriority = 5;
	*BottomSprite = *TopSprite;
	BottomSprite->FlipRule = SDL_FLIP_NONE;
	TopCollision->Rect = &TopSprite->DstRect;
	TopCollision->CollisionPin = (uint8_t)0x00000010;
	TopCollision->CollisionBitmask = (uint8_t)0x00000000;
	auto onCollision = [this](SDL_Rect* other)
	{
		Movement->bActive = false;
		TopCollision->bActive = false;
		BottomCollision->bActive = false;
		std::cout << "Pipe Collision" << std::endl;
	};
	TopCollision->OnCollision = onCollision;

	BottomSprite->RenderPriority = TopSprite->RenderPriority;
	BottomCollision->Rect = &BottomSprite->DstRect;
	BottomCollision->CollisionPin = TopCollision->CollisionPin;
	BottomCollision->CollisionBitmask = TopCollision->CollisionBitmask;
	BottomCollision->OnCollision = onCollision;

	Movement->HMoveSpeed = -150;
	Movement->Rects = {
		&TopSprite->DstRect,
		&BottomSprite->DstRect
	};

	Relocator->LimitX = -60;
	Relocator->Rects = {
		&TopSprite->DstRect,
	};
	Relocator->CheckPosition = [this](SDL_Rect* outRect) 
	{ 
		return TopSprite->DstRect.x <= Relocator->LimitX;
	};
	Relocator->RelocateSprites = [this](SDL_Rect* inRect)
	{
		TopSprite->DstRect.x = BottomSprite->DstRect.x = 280;

		int newBY = rand() % 200 + 100;
		int pipesOffset = rand() % 50 + 70;
		int newTY = newBY - pipesOffset - pipeHeight;
		BottomSprite->DstRect.y = newBY;
		TopSprite->DstRect.y = newTY;
	};

	MainObject->BindComponent<SpriteComponent>("Sprite", "Top Pipe Sprite", TopSprite);
	MainObject->BindComponent<SpriteComponent>("Sprite", "Bottom Pipe Sprite", BottomSprite);
	auto rm = RenderManager::Get();
	rm->RegisterSprite(RenderLayer::Environment, TopSprite);
	rm->RegisterSprite(RenderLayer::Environment, BottomSprite);
	MainObject->BindComponent<MovableComponent>("Movable", "Pipes Movement", Movement);
	MainObject->BindComponent<CollisionComponent>("Collision", "Top Pipe Collider", TopCollision);
	MainObject->BindComponent<CollisionComponent>("Collision", "Bottom Pipe Collider", BottomCollision);
	MainObject->BindComponent<RelocatableComponent>("Relocator", "Relocator", Relocator);
}

int PipesPairObjPack::Update(const double deltatime)
{
	return 0;
}

void PipesPairObjPack::LocatePipes()
{
	TopSprite->DstRect.x = BottomSprite->DstRect.x = 280;

	int newBY = rand() % 100 + 200;
	int pipesOffset = rand() % 50 + 100;
	int newTY = newBY - pipesOffset - pipeHeight;
	BottomSprite->DstRect.y = newBY;
	TopSprite->DstRect.y = newTY;
}
