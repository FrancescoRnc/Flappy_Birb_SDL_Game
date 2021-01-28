#include "ObjectPacks.h"



PlayerObjPack::PlayerObjPack()
{
	MainObject = new GameObject("Player");
	Sprite = new SpriteComponent(nullptr, FileManager::Get()->
								 GetTexture("redbird-midflap"),
								 {}, {70, 200}, nullptr, SDL_FLIP_NONE);
	Animator = new AnimatorComponent({FileManager::Get()->GetTexture("redbird-upflap"),
									  FileManager::Get()->GetTexture("redbird-midflap"),
									  FileManager::Get()->GetTexture("redbird-downflap")}, true);
	Movement = new MovableComponent();
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
	Sprite->RenderPriority = 20;
	Movement->Rects = {&Sprite->DstRect};
	Gravity->GravityForce = 98.1f * 5.5f;
	Gravity->Movable = Movement;
	Gravity->bActive = false;

	Collision->Rect = &Sprite->DstRect;
	Collision->CollisionPin = 0x00000001;
	Collision->CollisionBitmask = 0x00000110;
	Collision->OnCollision = [this](SDL_Rect* other)
	{
		OnGameOver();
		Flap->DoFlap->Action();
		GameOverSprite->bActive = true;
		//Gravity->bActive = false;
		Collision->bActive = false;
		//Movement->bActive = false;
		Flap->bActive = false;
		std::cout << "Player Collision" << std::endl;
	};

	Flap->Gravity = Gravity; 
	Flap->FlapForce = 180;
	Flap->DoFlap->Action = [this]()
	{
		Gravity->bActive = true;
		Movement->VMoveSpeed = -Flap->FlapForce;
		std::cout << "Flap Flap!" << std::endl;
	};


	GameOverSprite->bActive = false; GameOverSprite->RenderPriority = 100;
	OnGameOver = [this]()
	{
		auto components = ComponentManager::Get()->ComponentMap["Movable"];
		for (auto c : components) c->bActive = false;
		Movement->bActive = true;
	};

	MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite", Sprite);
	//MainObject->BindComponent<AnimatorComponent>("Animator", "Flap Animation", Animator);
	MainObject->BindComponent<MovableComponent>("Movable", "Movement", Movement);
	MainObject->BindComponent<CollisionComponent>("Collision", "Collider", Collision);
	MainObject->BindComponent<GravityComponent>("Gravity", "Gravity",  Gravity);
	MainObject->BindComponent<FlapComponent>("Flap", "Flap",  Flap);
	GameOver->BindComponent<SpriteComponent>("Sprite", "Game Over Sprite", GameOverSprite);
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
	Movement->HMoveSpeed = -60;
	Collision->Rect = &BaseSprite->DstRect;
	Collision->CollisionPin = 0x00000100;
	Collision->CollisionBitmask = 0x00000001;
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
	MainObject->BindComponent<MovableComponent>("Movable", "Background Movement", Movement);
	MainObject->BindComponent<CollisionComponent>("Collision", "Base Collider", Collision);
	MainObject->BindComponent<RelocatableComponent>("Relocator", "Relocator", BGRelocator);
}

PipesPairObjPack::PipesPairObjPack()
{
	MainObject = new GameObject("Pipes");

	TopSprite = new SpriteComponent(nullptr, FileManager::Get()->
									  GetTexture("pipe-green"),
									  {}, {}, nullptr, SDL_FLIP_VERTICAL);
	TopCollision = new CollisionComponent();
	
	BottomSprite = new SpriteComponent(/*nullptr, FileManager::Get()->
									   GetTexture("pipe-green"),
									   {}, {}, nullptr, SDL_FLIP_NONE*/);
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
	TopCollision->CollisionPin = 0x00000010;
	TopCollision->CollisionBitmask = 0x00000001;
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

	Movement->HMoveSpeed = -80;
	Movement->Rects = {
		&TopSprite->DstRect,
		&BottomSprite->DstRect
	};

	Relocator->LimitX = -60;
	Relocator->Rects = {
		&TopSprite->DstRect,
		//&BottomSprite->DstRect
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
	MainObject->BindComponent<MovableComponent>("Movable", "Pipes Movement", Movement);
	MainObject->BindComponent<CollisionComponent>("Collision", "Top Pipe Collider", TopCollision);
	MainObject->BindComponent<CollisionComponent>("Collision", "Bottom Pipe Collider", BottomCollision);
	MainObject->BindComponent<RelocatableComponent>("Relocator", "Relocator", Relocator);
}

void PipesPairObjPack::LocatePipes()
{
	TopSprite->DstRect.x = BottomSprite->DstRect.x = 280;

	int newBY = rand() % 100 + 200;
	int pipesOffset = rand() % 50 + 70;
	int newTY = newBY - pipesOffset - pipeHeight;
	BottomSprite->DstRect.y = newBY;
	TopSprite->DstRect.y = newTY;
}

bool PipesPairObjPack::CheckPipesX()
{
	return TopSprite->DstRect.x <= -60;
}
