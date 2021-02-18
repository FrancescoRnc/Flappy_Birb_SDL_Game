#pragma once
#include <vector>
#include "ComponentManager.h"
#include "ObjectPacks.h"

struct Engine;

// You should use this class to write Your effective Gameplay Design

class GameEditor
{
    public:

    std::vector<IObjectPack*> LoadObjects();

    Engine* refEngine;

    // Put here your Object Packs
    PlayerObjPack* player;
    std::vector<PipesPairObjPack*> pipesPairs;
    BackgroundObjPack* background;
    ScoreObjPack* score;
    ScoreCounterObjPack* counter;
    
    int GameStart();

    void OnGameOver();

    void StopMovingObjects();
};

