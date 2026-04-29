#include "TestSoundCommand.hpp"

#include <filesystem>

#include "Core/HelperFunctions.h"
#include "Managers/ResourceManager.h"


Game::TestSoundCommand::TestSoundCommand(bae::GameObject& owner, const std::filesystem::path& soundPath) :
    GameActorCommand(owner)
{
    if(!MIX_Init())
    {
        std::cout << "Failed to initialize MIX: " << SDL_GetError() << '\n';
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to initialize MIX: ") + SDL_GetError());
    }

    m_Mixer = std::unique_ptr<MIX_Mixer, MIX_MixerDeletor>(
        MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr));
    if(!m_Mixer)
    {
        std::cout << "Failed to make a MixerDevice Error: " << SDL_GetError() << '\n';
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to make a MixerDevice Error: ") + SDL_GetError());
    }

    const std::string beepSoundPath = std::filesystem::absolute(
        bae::ResourceManager::GetInstance().GetResourcesPath() / "Sounds/beep.mp3").string();

    m_Audio = std::unique_ptr<MIX_Audio, MIX_AudioDeletor>(MIX_LoadAudio(m_Mixer.get(), beepSoundPath.c_str(), false));
    if(!m_Audio)
    {
        std::cout << beepSoundPath << '\n';
        std::cout << "Failed to load Audio, Error: " << SDL_GetError() << '\n';
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to load Audio, Error: ") + SDL_GetError());
    }

    m_Track = std::unique_ptr<MIX_Track, MIX_TrackDeletor>(MIX_CreateTrack(m_Mixer.get()));
    if(!m_Track)
    {
        std::cout << "Failed to load Track, Error: " << SDL_GetError() << '\n';
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to load Track, Error: ") + SDL_GetError());
    }

    m_bIsMixerValid = MIX_SetTrackAudio(m_Track.get(), m_Audio.get());
}

void Game::TestSoundCommand::Execute()
{
    if(m_bIsMixerValid && m_Track)
    {
        MIX_PlayTrack(m_Track.get(), 0);
    }
}
