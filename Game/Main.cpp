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
#include "Base/SoundAssets.hpp"
#include "Commands/MoveCommand.hpp"
#include "Commands/MoveOnGridCommand.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/MsPacmanComponent.hpp"
#include "Components/RotateComponent.hpp"


namespace fs = std::filesystem;

void Start();
void LoadBackground();
void LoadGameNameScene();
void LoadFpsCounterScene();
void LoadRotatingObjectsScene();

void LoadSounds();
void TestSoundSystem();
void TestSoLoudSoundSystem();
void LoadMsPacman();


int main(int, char*[])
{
    #ifdef WIN32
    if(AllocConsole())
    {
        FILE* pEmpty;
        freopen_s(&pEmpty, "CONOUT$", "w", stdout);
        freopen_s(&pEmpty, "CONOUT$", "w", stderr);
    }
    #endif


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
    LoadSounds();

    LoadBackground();
    LoadFpsCounterScene();
    LoadGameNameScene();
    LoadRotatingObjectsScene();

    LoadMsPacman();
}

void LoadBackground()
{
    auto& backgroundScene = bae::SceneManager::GetInstance().CreateScene("Background Scene");

    const auto backgroundTexture = std::make_shared<bae::GameObject>("BackgroundTexture");
    backgroundTexture->AddComponent<bae::TextureComponent>(*backgroundTexture, "Textures/background.png");
    backgroundScene.Add(backgroundTexture);


    const auto backgroundLogoTexture = std::make_shared<bae::GameObject>("Background Logo Texture");
    backgroundLogoTexture->AddComponent<bae::TextureComponent>(*backgroundLogoTexture, "Textures/logo.png");
    const auto backgroundLogotTextureComp = backgroundLogoTexture->GetComponent<bae::TextureComponent>();

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    backgroundLogotTextureComp->m_bIsCenteredAtPosition = true;
    backgroundLogoTexture->SetWorldLocation(
        {
            static_cast<float>(windowSize.Width) / 2,
            static_cast<float>(windowSize.Height) / 2
        }
    );

    backgroundScene.Add(backgroundLogoTexture);

    bae::Utils::DrawCircle({ 0, 0 }, 1000, bae::Utils::Color::Blue);
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

void LoadRotatingObjectsScene()
{
    const auto& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto& ballScene = bae::SceneManager::GetInstance().CreateScene("Rotating Ball Scene");

    constexpr float moveSpeed        = 800.f;
    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    // LonelyRotatingBall
    const auto lonelyParent = std::make_shared<bae::GameObject>("LonelyRotatingBall Parent");
    lonelyParent->SetWorldLocation(
        glm::vec2(static_cast<float>(windowSize.Width) / 2, static_cast<float>(windowSize.Height) / 2));

    const auto lonelyRotatingBall = std::make_shared<bae::GameObject>("LonelyRotatingBall");
    lonelyRotatingBall->AddComponent<bae::TextureComponent>(*lonelyRotatingBall, "Textures/SpriteExample.png");
    lonelyRotatingBall->AddComponent<Game::RotateComponent>(*lonelyRotatingBall, 100.f, 1.f);

    lonelyParent->AttachChild(lonelyRotatingBall.get());

    ballScene.Add(lonelyParent);
    ballScene.Add(lonelyRotatingBall);

    // Lonely Parent Commands
    auto moveUpLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(
        *lonelyParent, Game::Direction::Up, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveUpLonelyRotatingBallParentCommand), SDLK_W,
                                 bae::InputManager::ButtonState::Pressed);

    auto moveDownLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(
        *lonelyParent, Game::Direction::Down, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveDownLonelyRotatingBallParentCommand), SDLK_S,
                                 bae::InputManager::ButtonState::Pressed);

    auto moveLeftLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(
        *lonelyParent, Game::Direction::Left, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveLeftLonelyRotatingBallParentCommand), SDLK_A,
                                 bae::InputManager::ButtonState::Pressed);

    auto moveRightLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(
        *lonelyParent, Game::Direction::Right, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveRightLonelyRotatingBallParentCommand), SDLK_D,
                                 bae::InputManager::ButtonState::Pressed);


    // Grouped Parent
    const auto groupedParent = std::make_shared<bae::GameObject>("Grouped Main Parent");
    groupedParent->SetWorldLocation(
        glm::vec2(static_cast<float>(windowSize.Width) / 2 - 200.f, static_cast<float>(windowSize.Height) / 2));
    ballScene.Add(groupedParent);

    // Grouped Parent Commands
    auto moveUpGroupedParentCommand = std::make_unique<Game::MoveCommand>(
        *groupedParent, Game::Direction::Up, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveUpGroupedParentCommand), SDLK_I,
                                 bae::InputManager::ButtonState::Pressed);

    auto moveDownGroupedParentCommand = std::make_unique<Game::MoveCommand>(
        *groupedParent, Game::Direction::Down, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveDownGroupedParentCommand), SDLK_K,
                                 bae::InputManager::ButtonState::Pressed);

    auto moveLeftGroupedParentCommand = std::make_unique<Game::MoveCommand>(
        *groupedParent, Game::Direction::Left, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveLeftGroupedParentCommand), SDLK_J,
                                 bae::InputManager::ButtonState::Pressed);

    auto moveRightGroupedParentCommand = std::make_unique<Game::MoveCommand>(
        *groupedParent, Game::Direction::Right, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveRightGroupedParentCommand), SDLK_L,
                                 bae::InputManager::ButtonState::Pressed);

    // Grouped Rotating Ball
    const auto groupedRotatingBall = std::make_shared<bae::GameObject>("Grouped Rotating Ball");
    groupedRotatingBall->AddComponent<bae::TextureComponent>(*groupedRotatingBall, "Textures/SpriteExample.png");
    groupedRotatingBall->AddComponent<Game::RotateComponent>(*groupedRotatingBall, 10.f, 5.f);

    groupedParent->AttachChild(groupedRotatingBall.get());

    ballScene.Add(groupedRotatingBall);

    // Grouped Rotating Child Ball
    const auto groupedRotatingChildBall = std::make_shared<bae::GameObject>("Grouped Rotating Child Ball");
    groupedRotatingChildBall->AddComponent<bae::TextureComponent>(*groupedRotatingChildBall,
                                                                  "Textures/SpriteExample.png");
    groupedRotatingChildBall->AddComponent<Game::RotateComponent>(*groupedRotatingChildBall, 40.f, -2.f);

    groupedRotatingBall->AttachChild(groupedRotatingChildBall.get());

    ballScene.Add(groupedRotatingChildBall);
}


void LoadSounds()
{
    namespace gs = Game::Sounds;

    #if __EMSCRIPTEN__

    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::LoggingSoundSystem>(
        std::make_unique<bae::SoLoudSoundSystem>()));

    #else

    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::LoggingSoundSystem>(
        std::make_unique<bae::MixerSoundSystem>()));

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


void LoadMsPacman()
{
    auto& msPacmanScene = bae::SceneManager::GetInstance().CreateScene("MsPacman Scene");

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();
    const auto msPacman              = std::make_shared<bae::GameObject>("MsPacman");
    msPacman->SetWorldLocation(
        glm::vec2(static_cast<float>(windowSize.Width) / 2, static_cast<float>(windowSize.Height) / 2));

    msPacman->AddComponent<Game::MsPacmanComponent>(*msPacman);

    constexpr glm::vec2 dimensions = { 50, 50 };
    constexpr glm::vec2 offset     = { 50, 20 };

    msPacman->AddComponent<Game::HitboxComponent>(*msPacman, dimensions, offset);
    const auto hitboxComp = msPacman->GetComponent<Game::HitboxComponent>();
    hitboxComp->SetVisibility(true);

    // Controls
    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    constexpr float msPacmanSpeed = 100.f;
    auto moveLeftCommand = std::make_unique<Game::MoveCommand>(*msPacman, Game::Direction::Left, msPacmanSpeed);
    auto moveRightCommand = std::make_unique<Game::MoveCommand>(*msPacman, Game::Direction::Right, msPacmanSpeed);
    auto moveDownCommand = std::make_unique<Game::MoveCommand>(*msPacman, Game::Direction::Down, msPacmanSpeed);
    auto moveUpCommand = std::make_unique<Game::MoveCommand>(*msPacman, Game::Direction::Up, msPacmanSpeed);

    keyboard.AddKeyboardCommands(std::move(moveLeftCommand), SDLK_A, bae::InputManager::ButtonState::Pressed);
    keyboard.AddKeyboardCommands(std::move(moveRightCommand), SDLK_D, bae::InputManager::ButtonState::Pressed);
    keyboard.AddKeyboardCommands(std::move(moveDownCommand), SDLK_S, bae::InputManager::ButtonState::Pressed);
    keyboard.AddKeyboardCommands(std::move(moveUpCommand), SDLK_W, bae::InputManager::ButtonState::Pressed);

    msPacmanScene.Add(msPacman);
}

