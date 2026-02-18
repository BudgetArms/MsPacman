#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <memory>
#include <cassert>

#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <chrono>


#pragma region BudgetArmsEngine Includes


#include <glm.hpp>
#include <SDL.h>
#include <imgui.h>
#include <imgui_plot.h>

#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif


#include "Core/BudgetEngine.h"
#include "Core/GameObject.h"
#include "Core/Renderer.h"
#include "Core/RingBuffer.h"
#include "Core/Scene.h"
#include "Core/ServiceLocator.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"

#include "Commands/Command.h"
#include "Commands/GameActorCommand.h"

#include "Components/Component.h"
#include "Components/FpsCounterComponent.h"
#include "Components/ImguiComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TrashTheCacheComponent.h"

#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"

#include "Sounds/SoundSystem.h"
#include "Sounds/LoggingSoundSystem.h"
#include "Sounds/SdlSoundSystem.h"
#include "Sounds/SdlAudioClip.h"
#include "Sounds/LoggingAudioClip.h"
#include "Sounds/NullAudioClip.h"

#include "Wrappers/Controller.h"
#include "Wrappers/Keyboard.h"
#include "Wrappers/Mouse.h"
#include "Wrappers/Texture2D.h"


#ifdef STEAMWORKS_ENABLED

#include "Core/Achievement.h"
#include "Managers/SteamManager.h"
#include "Managers/AchievementManager.h"

#endif


#pragma endregion



namespace fs = std::filesystem;
using namespace bae;

void Start();
void LoadFpsCounterScene();


int main(int, char* [])
{
    //thx myself for dying to find this
#ifdef WIN32
    if (AllocConsole())
    {
        FILE* pEmpty;
        freopen_s(&pEmpty, "CONOUT$", "w", stdout);
        freopen_s(&pEmpty, "CONOUT$", "w", stderr);
    }
#endif


    bae::Utils::Window window{ "Ms Pacman", "./Resources/", 960, 612, false };


#if __EMSCRIPTEN__
    window.resourceFolder = "";
#else

    if (!fs::exists(window.resourceFolder))
        window.resourceFolder = "../Resources/";

    if (!fs::exists(window.resourceFolder))
    {
        std::cout << "Resouces Folder Not Found" << '\n';
        assert("Resouces Folder Not Found");
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
}


void LoadFpsCounterScene()
{
    auto& fpsScene = SceneManager::GetInstance().CreateScene("FpsCounterScene");

    //auto font = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 36);
    //auto fontSmall = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);
    auto fontSmall = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

    auto fpsCounter = std::make_shared<GameObject>("Fps Counter");
    fpsCounter->AddComponent<FpsTextComponent>(*fpsCounter, fontSmall, SDL_Color(255, 255, 255, 255));

    SDL_Window* window = Renderer::GetInstance().GetSDLWindow();
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    fpsCounter->SetWorldLocation({ width, 0.f });
    fpsCounter->AddLocation({ -75.f, 5.f });

    fpsScene.Add(fpsCounter);


}
