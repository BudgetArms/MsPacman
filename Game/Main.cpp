// VLD include
#if _DEBUG && __has_include(<vld.h>)
    #include <vld.h>
#endif

// STD includes
#include <memory>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <chrono>


// External libraries includes
#if WIN32
    #include <Windows.h>
#endif

#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_plot.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

#ifdef STEAMWORKS_ENABLED
    #pragma warning (push)
    #pragma warning (disable: 4996)
    #include <steam_api.h>
    #pragma warning (pop)
#endif


// BudgetArmsEngine includes
#include "Core/BudgetEngine.h"
#include "Core/GameObject.h"
#include "Core/Renderer.h"
#include "Core/Scene.h"

#include "Components/TextComponent.h"
#include "Components/FpsCounterComponent.h"

#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"

#include "Wrappers/Controller.h"
#include "Wrappers/Texture2D.h"


#ifdef STEAMWORKS_ENABLED
    #include "Core/Achievement.h"

    #include "Managers/AchievementManager.h"
    #include "Managers/SteamManager.h"
#endif



namespace fs = std::filesystem;
using namespace bae;

void Start();
void LoadFpsCounterScene();
void PlayBeepSound();


int main(int, char* [])
{
    #ifdef WIN32
        if (AllocConsole())
        {
            FILE* pEmpty;
            freopen_s(&pEmpty, "CONOUT$", "w", stdout);
            freopen_s(&pEmpty, "CONOUT$", "w", stderr);
        }
    #endif

    #if defined(_DEBUG) && __has_include(<vld.h>)
        std::cout << "VLD enabled" << '\n';
    #else
        std::cout << "VLD disabled" << '\n';
    #endif

    bae::Utils::Window window{ "Ms Pacman", "./Resources/", 960, 612, false };


    #if __EMSCRIPTEN__
        window.resourceFolder = "";
    #else

        if (!fs::exists(window.resourceFolder))
            window.resourceFolder = "../Resources/";

        if (!fs::exists(window.resourceFolder))
        {
            std::cout << "Resources Folder Not Found" << '\n';
            assert("Resources Folder Not Found");
            exit(-1);
        }

        #ifdef STEAMWORKS_ENABLED
            SteamManager::GetInstance().Initialize();
        #endif

    #endif


    BudgetEngine engine(window);
    engine.Run(Start);


#ifdef STEAMWORKS_ENABLED
    SteamManager::GetInstance().Shutdown();
#endif

    std::cout << "\n\n";

    return 0;
}


void Start()
{
    LoadFpsCounterScene();
    PlayBeepSound();

}


void LoadFpsCounterScene()
{
    auto& fpsScene = SceneManager::GetInstance().CreateScene("FpsCounterScene");

    auto font = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 36);
    auto fontSmall = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);

    const auto fpsCounter = std::make_shared<GameObject>("Fps Counter");
    fpsCounter->AddComponent<FpsTextComponent>(*fpsCounter, fontSmall, SDL_Color(255, 255, 255, 255));

    SDL_Window* window = Renderer::GetInstance().GetSDLWindow();
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    fpsCounter->SetWorldLocation({ width, 0.f });
    fpsCounter->AddLocation({ -75.f, 5.f });

    fpsScene.Add(fpsCounter);
}


void PlayBeepSound()
{
    if (!MIX_Init())
    {
        std::cout << "Failed to initialize MIX: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to initialize MIX: " + std::string(SDL_GetError()));
    }


    MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer)
    {
        std::cout << "Failed to make a MixerDevice Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to make a MixerDevice Error: " + std::string(SDL_GetError()));
    }

    const std::string beepSoundPath = std::filesystem::absolute(bae::ResourceManager::GetInstance().GetResourcesPath() / "Sounds/beep.mp3").string();

    MIX_Audio* audio = MIX_LoadAudio(mixer, beepSoundPath.c_str(), false);
    if (!audio)
    {
        std::cout << beepSoundPath << std::endl;
        std::cout << "Failed to load Audio, Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to load Audio, Error: " + std::string(SDL_GetError()));
    }

    MIX_Track* track = MIX_CreateTrack(mixer);
    if (!track)
    {
        std::cout << "Failed to load Track, Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to load Track, Error: " + std::string(SDL_GetError()));
    }

    MIX_SetTrackAudio(track, audio);
    MIX_PlayTrack(track, 0);

}

