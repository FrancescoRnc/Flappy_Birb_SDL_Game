#pragma once

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

class IObjectPack : public IUpdatable
{
    public:

    virtual void Load() = 0;

    virtual int Update(const double deltatime) = 0;
};

class IInputHandler : public IInitiable
{
    public:

    virtual int Initialize() = 0;

    virtual int Loop() = 0;
};