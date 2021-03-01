#include "GameEditor.h"

std::vector<IObjectPack*> GameEditor::LoadObjects()
{
    player = new PlayerObjPack();
	player->Collision->OnCollision = [this](SDL_Rect* other)
	{
		Mix_PlayChannel(-1, AudioSystem::Get()->GetTrack("hit"), 0);
		//player->Flap->DoFlap->ActionStack.top();
		player->GameOverSprite->bActive = true;
		player->Collision->bActive = false;
		player->Flap->bActive = false;
		OnGameOver();
	};

 	pipesPairs = {
		new PipesPairObjPack(/*300, 0*/),
		new PipesPairObjPack(/*450, 0*/),
		new PipesPairObjPack(/*600, 0*/),
	};

	background = new BackgroundObjPack();

	score = new ScoreObjPack(pipesPairs);
	score->ScoreCollision->OnCollision = [this](SDL_Rect* other)
	{
		score->IncreaseScore();
		score->CurrentPipesPair = score->GetNextPair();
	};
	counter = new ScoreCounterObjPack();
	ScoreBridge* Sbridge = new ScoreBridge(score, counter);

	// Collisions
	auto physMgr = PhysicsManager::Get();
	physMgr->Collisions.insert({{0x00000001, 0x00000010},
							   {player->Collision->OnCollision, [this](SDL_Rect*) {}}});
	physMgr->Collisions.insert({{0x00000001, 0x00000100},
							   {player->Collision->OnCollision, [this](SDL_Rect*) {}}});
	physMgr->Collisions.insert({{0x00000001, 0x00001000}, 
							   {[this](SDL_Rect*) {}, score->ScoreCollision->OnCollision}});

	std::vector<IObjectPack*> objects = {
		player,
		pipesPairs[0],
		pipesPairs[1],
		pipesPairs[2],
		score,
		counter,
		background
	};

    return objects;
}

//int GameEditor::GameStart(Engine* engine)
//{
//	
//
//	return 0;
//}
