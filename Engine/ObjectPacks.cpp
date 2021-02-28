#include "ObjectPacks.h"
#include <cmath>

PlayerObjPack::PlayerObjPack()
{
	MainObject = new GameObject("Player");
	Animator = new AnimatorComponent(MainObject, {FileManager::Get()->
									  GetTexture("redbird-upflap"),
									  FileManager::Get()->
									  GetTexture("redbird-midflap"),
									  FileManager::Get()->
									  GetTexture("redbird-downflap"),
									  FileManager::Get()->
									  GetTexture("redbird-midflap")});
	Sprite = new SpriteComponent(MainObject, nullptr, Animator->textures[0],
								 {}, {}, nullptr, SDL_FLIP_NONE);
	Movement = new MovableComponent(MainObject);
	Rotator = new RotatorComponent(MainObject);
	Gravity = new GravityComponent(MainObject);
	Collision = new CollisionComponent(MainObject);
	Flap = new FlapComponent(MainObject);
	Flap->DoFlap = new InputAction();

	GameOver = new GameObject("Game Over");
	GameOverSprite = new SpriteComponent(GameOver, nullptr, FileManager::Get()->
										 GetTexture("gameover"),
										 {}, {39, 100}, nullptr, SDL_FLIP_NONE);
}

void PlayerObjPack::Load(ComponentManager* mgr)
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
	Flap->DoFlap->ActionStack.push(Flap->Func);

	GameOverSprite->bActive = false; GameOverSprite->RenderPriority = 100;

	auto rm = RenderManager::Get();
	rm->RegisterSprite(RenderLayer::Player, Sprite);
	rm->RegisterSprite(RenderLayer::HUD, GameOverSprite);

	mgr->SpriteContainer.AddBy(MainObject->Entity, Sprite);
	mgr->SpriteContainer.AddBy(GameOver->Entity, GameOverSprite);
	mgr->AnimatorContainer.AddBy(MainObject->Entity, Animator);
	mgr->MovableContainer.AddBy(MainObject->Entity, Movement);
	mgr->RotatorContainer.AddBy(MainObject->Entity, Rotator);
	mgr->CollisionContainer.AddBy(MainObject->Entity, Collision);
	mgr->GravityContainer.AddBy(MainObject->Entity, Gravity);
	mgr->FlapContainer.AddBy(MainObject->Entity, Flap);

	//MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite", Sprite);
	//GameOver->  BindComponent<SpriteComponent>("Sprite", "Game Over Sprite", GameOverSprite);
	//MainObject->BindComponent<AnimatorComponent>("Animator", "Flap Animation", Animator);
	//MainObject->BindComponent<MovableComponent>("Movable", "Movement", Movement);
	//MainObject->BindComponent<RotatorComponent>("Rotator", "Rotator", Rotator);
	//MainObject->BindComponent<CollisionComponent>("Collision", "Collider", Collision);
	//MainObject->BindComponent<GravityComponent>("Gravity", "Gravity",  Gravity);
	//MainObject->BindComponent<FlapComponent>("Flap", "Flap",  Flap);	 
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
	BGSprite = new SpriteComponent(MainObject, nullptr, FileManager::Get()->
								   GetTexture("background-day-extend"),
								   {}, {}, nullptr, SDL_FLIP_NONE);
	BaseSprite = new SpriteComponent(MainObject, nullptr, FileManager::Get()->
									 GetTexture("base-extend"),
									 {}, {0, 400}, nullptr, SDL_FLIP_NONE);
	Movement = new MovableComponent(MainObject);
	Collision = new CollisionComponent(MainObject);
	BGRelocator = new RelocatableComponent(MainObject);
}

void BackgroundObjPack::Load(ComponentManager* mgr)
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


	auto rm = RenderManager::Get();
	rm->RegisterSprite(RenderLayer::Background, BGSprite);
	rm->RegisterSprite(RenderLayer::BackgroundBase, BaseSprite);

	mgr->SpriteContainer.AddBy(MainObject->Entity, BGSprite);
	mgr->SpriteContainer.AddBy(MainObject->Entity, BaseSprite);
	mgr->MovableContainer.AddBy(MainObject->Entity, Movement);
	mgr->CollisionContainer.AddBy(MainObject->Entity, Collision);
	mgr->RelocatableContainer.AddBy(MainObject->Entity, BGRelocator);

	//MainObject->BindComponent<SpriteComponent>("Sprite", "Background Sprite", BGSprite);
	//MainObject->BindComponent<SpriteComponent>("Sprite", "Base Sprite", BaseSprite);
	//MainObject->BindComponent<MovableComponent>("Movable", "Background Movement", Movement);
	//MainObject->BindComponent<CollisionComponent>("Collision", "Base Collider", Collision);
	//MainObject->BindComponent<RelocatableComponent>("Relocator", "Relocator", BGRelocator);
}

int BackgroundObjPack::Update(const double deltatime)
{
	return 0;
}


PipesPairObjPack::PipesPairObjPack()
{
	MainObject = new GameObject("Pipes");

	TopSprite = new SpriteComponent(MainObject, nullptr, FileManager::Get()->
									GetTexture("pipe-green"),
									{}, {}, nullptr, SDL_FLIP_VERTICAL);
	TopCollision = new CollisionComponent(MainObject);
	
	BottomSprite = new SpriteComponent(MainObject);
	BottomCollision = new CollisionComponent(MainObject);
	
	Movement = new MovableComponent(MainObject);
	Relocator = new RelocatableComponent(MainObject);

	SDL_QueryTexture(TopSprite->Texture, nullptr, nullptr, nullptr, &pipeHeight);
}

PipesPairObjPack::PipesPairObjPack(int x, int y)
{
	MainObject = new GameObject("Pipes");

	TopSprite = new SpriteComponent(MainObject, nullptr, FileManager::Get()->
									GetTexture("pipe-green"),
									{}, {x, y}, nullptr, SDL_FLIP_VERTICAL);
	TopCollision = new CollisionComponent(MainObject);

	BottomSprite = new SpriteComponent(MainObject);
	BottomCollision = new CollisionComponent(MainObject);

	Movement = new MovableComponent(MainObject);
	Relocator = new RelocatableComponent(MainObject);

	SDL_QueryTexture(TopSprite->Texture, nullptr, nullptr, nullptr, &pipeHeight);
}

void PipesPairObjPack::Load(ComponentManager* mgr)
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

	auto rm = RenderManager::Get();
	rm->RegisterSprite(RenderLayer::Environment, TopSprite);
	rm->RegisterSprite(RenderLayer::Environment, BottomSprite);

	mgr->SpriteContainer.AddBy(MainObject->Entity, TopSprite);
	mgr->SpriteContainer.AddBy(MainObject->Entity, BottomSprite);
	mgr->MovableContainer.AddBy(MainObject->Entity, Movement);
	mgr->CollisionContainer.AddBy(MainObject->Entity, TopCollision);
	mgr->CollisionContainer.AddBy(MainObject->Entity, BottomCollision);
	mgr->RelocatableContainer.AddBy(MainObject->Entity, Relocator);

	//MainObject->BindComponent<SpriteComponent>("Sprite", "Top Pipe Sprite", TopSprite);
	//MainObject->BindComponent<SpriteComponent>("Sprite", "Bottom Pipe Sprite", BottomSprite);
	//MainObject->BindComponent<MovableComponent>("Movable", "Pipes Movement", Movement);
	//MainObject->BindComponent<CollisionComponent>("Collision", "Top Pipe Collider", TopCollision);
	//MainObject->BindComponent<CollisionComponent>("Collision", "Bottom Pipe Collider", BottomCollision);
	//MainObject->BindComponent<RelocatableComponent>("Relocator", "Relocator", Relocator);

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
	int newBY = rand() % 100 + 200;
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
	
	ScoreCollision = new CollisionComponent(MainObject);
}

void ScoreObjPack::Load(ComponentManager* mgr)
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

	mgr->CollisionContainer.AddBy(MainObject->Entity, ScoreCollision);

	//MainObject->BindComponent<CollisionComponent>("Collision", "Hitbox", ScoreCollision);
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

	SpriteUnit = new SpriteComponent(MainObject, nullptr, NumbersTex[0],
									 {}, {147, 30}, nullptr, SDL_FLIP_NONE);
	SpriteTen = new SpriteComponent(MainObject, nullptr, NumbersTex[0],
									{}, {123, 30}, nullptr, SDL_FLIP_NONE);
	SpriteHundred = new SpriteComponent(MainObject, nullptr, NumbersTex[0],
									    {}, {99, 30}, nullptr, SDL_FLIP_NONE);
}

void ScoreCounterObjPack::Load(ComponentManager* mgr)
{
	SpriteHundred->RenderPriority =
		SpriteTen->RenderPriority =
		SpriteUnit->RenderPriority = 100;

	RenderManager::Get()->RegisterSprite(RenderLayer::HUD, SpriteUnit);
	RenderManager::Get()->RegisterSprite(RenderLayer::HUD, SpriteTen);
	RenderManager::Get()->RegisterSprite(RenderLayer::HUD, SpriteHundred);

	mgr->SpriteContainer.AddBy(MainObject->Entity, SpriteUnit);
	mgr->SpriteContainer.AddBy(MainObject->Entity, SpriteTen);
	mgr->SpriteContainer.AddBy(MainObject->Entity, SpriteHundred);

	//MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite Unit", SpriteUnit);
	//MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite Ten", SpriteTen);
	//MainObject->BindComponent<SpriteComponent>("Sprite", "Sprite Hundred", SpriteHundred);
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
