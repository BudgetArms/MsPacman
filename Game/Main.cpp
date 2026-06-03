// VLD include
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

// STD includes
#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>


// External libraries includes
#if WIN32
#include <Windows.h>
#endif

#include <imgui.h>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Core/ServiceLocator.hpp"
#include "Sounds/LoggingSoundSystem.hpp"

#ifndef __EMSCRIPTEN__
#include "Sounds/MixerSoundSystem.hpp"
#endif
#include "Sounds/SoLoudSoundSystem.hpp"


#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

// BudgetArmsEngine includes
#include "Core/BudgetEngine.hpp"
#include "Core/GameObject.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"

#include "Components/FpsCounterComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Components/TextureComponent.hpp"

#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"

#include "Wrappers/Controller.hpp"
#include "Wrappers/Keyboard.hpp"
#include "Wrappers/Texture2D.hpp"


#ifdef STEAMWORKS_ENABLED
#include "Core/Achievement.hpp"
#include "Managers/AchievementManager.hpp"
#include "Managers/SteamManager.hpp"
#endif


// Game Includes
#include "Base/Events.hpp"
#include "Base/SoundAssets.hpp"
#include "Commands/TestMousePositionCommand.hpp"
#include "Commands/ToggleMuteAllSoundsCommand.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LevelGridComponent.hpp"
#include "Components/LevelManagerComponent.hpp"
#include "Wrappers/Mouse.hpp"


namespace fs = std::filesystem;

void Start();
void LoadBackground();
void LoadLevelManager();
void LoadGameNameScene();
void LoadFpsCounterScene();

void LoadSounds();

void LoadSoundCommands();
void EnableLogMousePosition();


int main(int, char*[])
{
    #if _DEBUG && __has_include(<vld.h>)
    std::cout << "VLD enabled" << '\n';
    #else
    std::cout << "VLD disabled" << '\n';
    #endif

    bae::Utils::Window window{ "Ms Pacman", "./Resources/", 1024, 576, false };


    #if __EMSCRIPTEN__
    window.ResourceFolder = "";
    #else

    if(!fs::exists(window.ResourceFolder))
    {
        window.ResourceFolder = "../Resources/";
    }

    if(!fs::exists(window.ResourceFolder))
    {
        std::cout << "Resources Folder Not Found" << '\n';
        assert("Resources Folder Not Found");
        exit(-1);
    }

    #ifdef STEAMWORKS_ENABLED
    bae::SteamManager::GetInstance().Initialize();
    #endif

    #endif


    bae::BudgetEngine engine(window);
    engine.Run(Start);


    #ifdef STEAMWORKS_ENABLED
    bae::SteamManager::GetInstance().Shutdown();
    #endif

    std::cout << "\n\n";
    return 0;
}


void Start()
{
    EnableLogMousePosition();

    LoadSounds();
    LoadLevelManager();
    // LoadLevelGrid();

    LoadFpsCounterScene();
    LoadGameNameScene();


    LoadSoundCommands();
}

void LoadBackground()
{
    auto& backgroundScene = bae::SceneManager::GetInstance().CreateScene("Background Scene");

    const auto backgroundTexture = std::make_shared<bae::GameObject>("BackgroundTexture");
    backgroundTexture->AddComponent<bae::TextureComponent>(*backgroundTexture, "Textures/background.png");
    backgroundScene.Add(backgroundTexture);


    const auto backgroundLogoTexture = std::make_shared<bae::GameObject>("Background Logo Texture");
    backgroundLogoTexture->AddComponent<bae::TextureComponent>(*backgroundLogoTexture, "Textures/logo.png");
    const auto backgroundLogoTextureComp = backgroundLogoTexture->GetComponent<bae::TextureComponent>();

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    backgroundLogoTextureComp->m_bIsCenteredAtPosition = true;
    backgroundLogoTexture->SetWorldLocation(
        {
            static_cast<float>(windowSize.Width) / 2,
            static_cast<float>(windowSize.Height) / 2
        }
    );

    backgroundScene.Add(backgroundLogoTexture);

    bae::Utils::DrawCircle({ 0, 0 }, 1000, bae::Utils::Color::Blue);
}

void LoadLevelManager()
{
    auto& managerComponentScene = bae::SceneManager::GetInstance().CreateScene(
        Game::LevelManagerComponent::m_LevelManagersSceneName.data());

    const auto levelManager = std::make_shared<bae::GameObject>("LevelManager");
    levelManager->AddComponent<Game::LevelManagerComponent>(*levelManager, Game::GameMode::Singleplayer);

    const auto levelManagerComponent =
            levelManager->GetComponent<Game::LevelManagerComponent>();

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();
    levelManagerComponent->SetSpriteSheetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });
    levelManagerComponent->SetSpriteSheetWorldScale({ 2.f, 2.f });

    levelManagerComponent->LoadLevelFromFile(0, "Levels/Level_1.json");
    // levelManagerComponent->CreateLevel();

    managerComponentScene.Add(levelManager);

    bae::EventQueue::GetInstance().SendEvent(Game::GetEventHash(Game::Events::BeginLevel));
}

void LoadGameNameScene()
{
    auto& gameNameScene = bae::SceneManager::GetInstance().CreateScene("Game Name Scene");
    auto font           = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 48);

    const auto gameName = std::make_shared<bae::GameObject>("Game Name");
    gameName->AddComponent<bae::TextComponent>(*gameName, "MsPacMan", font, bae::Utils::Color::Green);

    gameNameScene.Add(gameName);
}

void LoadFpsCounterScene()
{
    auto& fpsScene = bae::SceneManager::GetInstance().CreateScene("FpsCounterScene");

    auto fontSmall = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);

    const auto fpsCounter = std::make_shared<bae::GameObject>("Fps Counter");
    fpsCounter->AddComponent<bae::FpsTextComponent>(*fpsCounter, fontSmall, bae::Utils::Color::White);

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    fpsCounter->SetWorldLocation({ windowSize.Width, 0.f });
    fpsCounter->AddLocation({ -75.f, 5.f });

    fpsScene.Add(fpsCounter);
}


void LoadSounds()
{
    namespace gs = Game::Sounds;

    #if __EMSCRIPTEN__

    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::LoggingSoundSystem>(
        std::make_unique<bae::SoLoudSoundSystem>()));

    #else

    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::LoggingSoundSystem>(
        // std::make_unique<bae::MixerSoundSystem>()));
        std::make_unique<bae::NullSoundSystem>()));

    #endif

    const auto soundSystem = &bae::ServiceLocator::GetSoundSystem();

    // Sound files not made yet
    gs::g_sSoundEvents =
    {
        { gs::SoundAssets::BeepSound, soundSystem->LoadSound("Sounds/beep.wav") },
        { gs::SoundAssets::PlayerDeath, soundSystem->LoadSound("Sounds/AsmrVoice.wav") },
    };
    std::cout << '\n';
}

void LoadSoundCommands()
{
    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto toggleMuteAllSoundsCommand = std::make_unique<Game::ToggleMuteAllSoundsCommand>();
    keyboard.AddKeyboardCommands(std::move(toggleMuteAllSoundsCommand), SDLK_F2, bae::InputManager::ButtonState::Down);
}

void EnableLogMousePosition()
{
    const bae::Mouse& mouse = bae::InputManager::GetInstance().GetMouse();

    auto mousePressedCommand = std::make_unique<Game::TestMousePositionCommand>();
    mouse.AddMouseCommands(std::move(mousePressedCommand), SDL_BUTTON_RIGHT, bae::InputManager::ButtonState::Pressed);
}

