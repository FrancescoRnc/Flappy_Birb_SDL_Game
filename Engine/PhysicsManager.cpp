#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::_instance = nullptr;

PhysicsManager::PhysicsManager()
{
    _instance = this;
}

void PhysicsManager::AddCollider(CollisionComponent* collider)
{
    Colliders.push_back(collider);
}

int PhysicsManager::Update(const double deltatime)
{
    size_t colliders_size = Colliders.size();
    size_t actual_size = colliders_size - 1;
    for (size_t c = 0; c <= actual_size; c++)
    {
        auto coll = Colliders[c];
        if (coll->bActive)
        {
            for (size_t o = c+1; o<colliders_size; o++)
            {
                auto other = Colliders[o];
                if ((coll->CollisionBitmask&other->CollisionPin)&&
                    SDL_HasIntersection(coll->Rect, other->Rect))
                {
                    auto c = Collisions[{coll->CollisionPin, other->CollisionPin}];
                    c.first(coll->Rect);
                    c.second(coll->Rect);
                }
            }
        }        
    }
    return 0;
}
