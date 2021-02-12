#pragma once
#include <vector>

// Here are collected all the Interfaces

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

class GameObject;

class IObjectPack : public IUpdatable
{
    public:

    GameObject* MainObject;

    virtual void Load() = 0;

    virtual int Update(const double deltatime) = 0;
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