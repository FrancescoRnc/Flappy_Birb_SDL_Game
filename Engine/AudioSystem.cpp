#include "AudioSystem.h"

AudioSystem* AudioSystem::_instance = nullptr;

AudioSystem::AudioSystem()
{
	_instance = this;
}


AudioSystem::~AudioSystem()
{
	for (auto chunk : AudioTracks)
		Mix_FreeChunk(chunk.second);
	AudioTracks.clear();
	_instance = nullptr;
	Mix_CloseAudio();
}

int AudioSystem::Initialize()
{
    return Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}

int AudioSystem::LoadAudioFiles(const std::string path)
{
	auto files = std::filesystem::directory_iterator(path);
	for (auto file:files)
	{
		// Finding file path and a name to use for Map
		// Ex: "die.wav" -> name = "die"
		std::cout<<file.path()<<std::endl;
		auto name = file.path().string().substr(path.size());
		auto trueNameSize = name.size()-4;
		name = name.substr(0, trueNameSize);

		auto audio = Mix_LoadWAV(file.path().string().c_str());
		if (!audio) return 1;

		AudioTracks.insert({name, audio});
	}
	std::cout<<"Audio tracks loaded succesfully!"<<std::endl;
	std::cout<<std::endl;
    return 0;
}

Mix_Chunk* AudioSystem::GetTrack(const std::string name)
{
	return AudioTracks[name];
}
