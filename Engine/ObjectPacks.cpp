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
								 {}, {}, nullptr, SDL_FLIP_NONE);
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
	Animator->frame_max_time = 1. / 7.5;
	Movement->Rects = {&Sprite->DstRect};
	Rotator->MinAngle = -90; Rotator->MaxAngle = 90.;
	Sprite->RotationAngleDegree = Rotator->Rotation;
	Gravity->GravityForce = (98.1 * 10.);
	Gravity->refMovable = Movement;
	Gravity->bActive = false;

	Collision->Rect = &Sprite->DstRect;
	Collision->CollisionPin = 0x00000001;
	Collision->CollisionBitmask = 0x00000110;
	//Collision->OnCollision = [this](SDL_Rect* other)
	//{
	//	// FINISH GAME OVER 
	//
	//	
	//	Flap->DoFlap->ActionStack.top();
	//	GameOverSprite->bActive = true;
	//	Collision->bActive = false;
	//	Flap->bActive = false;
	//	//std::cout << "Player Collision" << std::endl;
	//};

	Flap->Gravity = Gravity; 
	Flap->FlapForce = 300.;
	Flap->KeyCode = SDLK_SPACE;
	Flap->Func = [this]()
	{
		Gravity->bActive = true;
		Movement->VMoveSpeed = -Flap->FlapForce;
		Mix_PlayChannel(-1, AudioSystem::Get()->GetTrack("wing"), 0);
		//std::cout << "Flap Flap!" << std::endl;
	};
	//Flap->DoFlap->ActionStack.push({});
	Flap->DoFlap->ActionStack.push(Flap->Func);

	GameOverSprite->bActive = false; GameOverSprite->RenderPriority = 100;
	//OnGameOver = [this]()
	//{
	//	Mix_PlayChannel(-1, AudioSystem::Get()->GetTrack("hit"), 0);
	//	Flap->Func();
	//	auto components = ComponentManager::Get()->ComponentMap["Movable"];
	//	for (auto c : components) c->bActive = false;
	//	Movement->bActive = true;
	//};

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
	return 0;
}

void PlayerObjPack::Locate(int x, int y)
{
	*MainObject->PosX = x;
	*MainObject->PosY = y;
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
		&BaseSprite->DstRect, 
	};
	Movement->HMoveSpeed = -150.;
	Collision->Rect = &BaseSprite->DstRect;
	Collision->CollisionPin = 0x00000100;
	Collision->CollisionBitmask = 0x00000001;
	Collision->OnCollision = [this](SDL_Rect* other)
	{
		//std::cout << "Base Collision" << std::endl;
	};
	BGRelocator->Rects = {
		&BaseSprite->DstRect,
	};
	BGRelocator->LimitX = -(BaseSprite->DstRect.w / 2);
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

PipesPairObjPack::PipesPairObjPack(int x, int y)
{
	MainObject = new GameObject("Pipes");

	TopSprite = new SpriteComponent(nullptr, FileManager::Get()->
									GetTexture("pipe-green"),
									{}, {x, y}, nullptr, SDL_FLIP_VERTICAL);
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

	Movement->bActive = false;
	Movement->HMoveSpeed = -150;
	Movement->Rects = {
		&TopSprite->DstRect,
		&BottomSprite->DstRect
	};

	Relocator->LimitX = -50;
	Relocator->Rects = {
		&TopSprite->DstRect,
	};
	Relocator->CheckPosition = [this](SDL_Rect* outRect) 
	{ 
		return TopSprite->DstRect.x <= Relocator->LimitX;
	};
	Relocator->RelocateSprites = [this](SDL_Rect* inRect)
	{
 		LocatePipes(inRect->x + 270 + 135 + 50);

		RandomizeY();
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

	RandomizeY();
}

int PipesPairObjPack::Update(const double deltatime)
{
	return 0;
}

void PipesPairObjPack::LocatePipes(int newX)
{
	TopSprite->DstRect.x = BottomSprite->DstRect.x = newX;
}

void PipesPairObjPack::LocatePipesAdditive(int addX)
{
	TopSprite->DstRect.x = BottomSprite->DstRect.x += addX;
}

void PipesPairObjPack::RandomizeY()
{
	int newBY = rand()%100+200;
	int pipesOffset = rand() % 50 + 100;
	int newTY = newBY - pipesOffset - pipeHeight;
	BottomSprite->DstRect.y = newBY;
	TopSprite->DstRect.y = newTY;
}

void PipesPairObjPack::Locate(int x, int y)
{
	TopSprite->DstRect.x = BottomSprite->DstRect.x = x;
	TopSprite->DstRect.y = BottomSprite->DstRect.y = y;	
	RandomizeY();
}


ScoreObjPack::ScoreObjPack(std::vector<PipesPairObjPack*> pairs)
{
	MainObject = new GameObject("Score Hitbox");

	Pairs = pairs;
	CurrentPipesPair = pairs[currentPipesIndex];
	
	ScoreCollision = new CollisionComponent();
}

void ScoreObjPack::Load()
{
	Hitbox = {0,0,30,400};
	ScoreCollision->Rect = &Hitbox;
	ScoreCollision->CollisionPin = (uint8_t)0x00001000;
	ScoreCollision->CollisionBitmask = (uint8_t)0x00000001;
	ScoreCollision->OnCollision = [this](SDL_Rect* other)
	{
		IncreaseScore();
		CurrentPipesPair = GetNextPair();
	};

	MainObject->BindComponent<CollisionComponent>("Collision", "Hitbox", ScoreCollision);
}

int ScoreObjPack::Update(const double deltatime)
{
	Hitbox.x = CurrentPipesPair->TopSprite->DstRect.x + 30;
	return 0;
}

void ScoreObjPack::StartwithFirstPair()
{
	currentPipesIndex = 0;
	CurrentPipesPair = Pairs[currentPipesIndex];
}

PipesPairObjPack* ScoreObjPack::GetNextPair()
{
	currentPipesIndex = (currentPipesIndex + 1) % Pairs.size();
	return Pairs[currentPipesIndex];
}

void ScoreObjPack::IncreaseScore()
{
    Mix_PlayChannel(-1, AudioSystem::Get()->GetTrack("point"), 0);
	ScorePoints += 1;
	Bridge->refCounter->ChangeNumber();
}

void ScoreObjPack::ResetScore()
{
	ScorePoints = 0;
	Bridge->refCounter->SpriteUnit->Texture = Bridge->refCounter->NumbersTex[0];
	Bridge->refCounter->SpriteTen->Texture = Bridge->refCounter->NumbersTex[0];
	Bridge->refCounter->SpriteHundred->Texture = Bridge->refCounter->NumbersTex[0];
}


ScoreCounterObjPack::ScoreCounterObjPack()
{
	MainObject = new GameObject("UI Score Counter");
	NumbersTex = {
		FileManager::Get()->GetTexture("0"),
		FileManager::Get()->GetTexture("1"),
		FileManager::Get()->GetTexture("2"),
		FileManager::Get()->GetTexture("3"),
		FileManager::Get()->GetTexture("4"),
		FileManager::Get()->GetTexture("5"),
		FileManager::Get()->GetTexture("6"),
		FileManager::Get()->GetTexture("7"),
		FileManager::Get()->GetTexture("8"),
		FileManager::Get()->GetTexture("9"),
	};

	SpriteUnit = new SpriteComponent(nullptr, NumbersTex[0],
									 {}, {147, 30}, nullptr, SDL_FLIP_NONE);
	SpriteTen = new SpriteComponent(nullptr, NumbersTex[0],
									{}, {123, 30}, nullptr, SDL_FLIP_NONE);
	SpriteHundred = new SpriteComponent(nullptr, NumbersTex[0],
									    {}, {99, 30}, nullptr, SDL_FLIP_NONE);
}

void ScoreCounterObjPack::Load()
{
	SpriteHundred->RenderPriority =
		SpriteTen->RenderPriority =
		SpriteUnit->RenderPriority = 100;

	MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite Unit", SpriteUnit);
	MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite Ten", SpriteTen);
	MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite Hundred", SpriteHundred);
	RenderManager::Get()->RegisterSprite(RenderLayer::HUD, SpriteUnit);
	RenderManager::Get()->RegisterSprite(RenderLayer::HUD, SpriteTen);
	RenderManager::Get()->RegisterSprite(RenderLayer::HUD, SpriteHundred);
}

int ScoreCounterObjPack::Update(const double deltatime)
{

	return 0;
}

void ScoreCounterObjPack::ChangeNumber()
{
	int unitIndex = Bridge->refScore->ScorePoints % 10;
	SpriteUnit->Texture = NumbersTex[unitIndex];
	
	if (Bridge->refScore->ScorePoints>=10)
	{
		unitIndex = (int)((Bridge->refScore->ScorePoints%100)/10);
		SpriteTen->Texture = NumbersTex[unitIndex];
	}
	
	if (Bridge->refScore->ScorePoints>=100)
	{
		unitIndex = (int)((Bridge->refScore->ScorePoints%1000)/100);
		SpriteHundred->Texture = NumbersTex[unitIndex];
	}
}
