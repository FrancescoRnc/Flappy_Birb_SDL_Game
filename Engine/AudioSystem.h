#pragma once
#include <iostream>
#include <filesystem>
#include <SDL.h>
#include <SDL_mixer.h>
#include "Interfaces.h"
#include "Component.h"

#define RULE_OF_FIVE(I) I(const I&) = delete; I(I&&) = delete; I operator= (const I&) = delete; I operator= (I&&) = delete; ~I();

class AudioSystem : public IInitiable
{
    public:

    AudioSystem();

    RULE_OF_FIVE(AudioSystem);

    std::map<std::string, Mix_Chunk*> AudioTracks;

    // Inherited via IInitiable
    virtual int Initialize() override;
    // - - - -

    int LoadAudioFiles(const std::string path);
    Mix_Chunk* GetTrack(const std::string name);

    static AudioSystem* Get()
    {
        return _instance;
    }

    private:

    static AudioSystem* _instance;
};

