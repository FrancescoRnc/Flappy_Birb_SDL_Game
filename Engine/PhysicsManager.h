#include <SDL.h>
#include <vector>
#include <functional>
#include "ComponentManager.h"

#pragma once
class PhysicsManager : public IUpdatable
{
    public:

    PhysicsManager();

    // List of all Collision Components used for the System
    std::vector<CollisionComponent*> Colliders = {};

    // List of all Collision Component that will not be used for the System
    // This will be used just to contains elements.
    std::vector<CollisionComponent*> ExcludedColliders = {};

    // this map calls a pair of functions that will be
    // the Collision event between the collided objects.
    // Key is a pair of the two Collision Pins;
    // Value is a pair of the two Collision Events;
    std::map<std::pair<unsigned int, unsigned int>,
             std::pair<std::function<void(SDL_Rect*)>, 
                       std::function<void(SDL_Rect*)>>> Collisions = {};

    void AddCollider(CollisionComponent* collider);

    // Inherited via IUpdatable
    virtual int Update(const double deltatime) override;


    static PhysicsManager* Get()
    {
        return _instance;
    }

    private:
    static PhysicsManager* _instance;
};

