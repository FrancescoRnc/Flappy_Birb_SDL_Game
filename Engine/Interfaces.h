#pragma once
#include <vector>

// Here are collected all the Interfaces

class ComponentManager;
class GameObject;


class IInitiable
{
    public:
    
    virtual int Initialize() = 0;
};

class IUpdatable
{
    public:

    virtual int Update(const double deltatime) = 0;
};

class IActivable
{
    public:

    //bool bActive;

    virtual void Active() = 0;

    virtual void Disactive() = 0;
};

class IObjectPack : public IUpdatable
{
    public:

    GameObject* MainObject;

    virtual void Load(ComponentManager* mgr) = 0;

    virtual int Update(const double deltatime) = 0;
};

class ILocatable
{
    public: 

    virtual void Locate(int x, int y) = 0;
};

class IInputHandler : public IInitiable
{
    public:

    virtual int Initialize() = 0;

    virtual int Loop() = 0;
};

class IScene : public IUpdatable
{
    public:

    virtual void Load(std::vector<IObjectPack*> objects) = 0;

    virtual void Unload() = 0;

    virtual int Update(const double deltatime) = 0;
};