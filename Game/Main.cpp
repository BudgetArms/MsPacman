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

#include "Components/FpsCounterComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"

#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"

#include "Wrappers/Controller.h"
#include "Wrappers/Keyboard.h"
#include "Wrappers/Texture2D.h"


#ifdef STEAMWORKS_ENABLED
    #include "Core/Achievement.h"

    #include "Managers/AchievementManager.h"
    #include "Managers/SteamManager.h"
#endif


// Game Includes
#include "Base/SmartpointerHelpers.hpp"
#include "Commands/MoveCommand.hpp"
#include "Commands/MoveOnGridCommand.hpp"
#include "Components/RotateComponent.hpp"


namespace fs = std::filesystem;
using namespace bae;

void Start();
void LoadBackground();
void LoadGameNameScene();
void LoadFpsCounterScene();
void LoadRotatingObjectsScene();
void PlayBeepSound();
void PlayBeepSoundWithAudioStream();


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
    bae::Utils::Window window{ "Ms Pacman", "./Resources/", 1024, 576, false };


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
    LoadBackground();
    LoadFpsCounterScene();
    LoadGameNameScene();
    LoadRotatingObjectsScene();
    PlayBeepSound();
    // PlayBeepSoundWithAudioStream();
}

void LoadBackground()
{
    auto& backgroundScene = SceneManager::GetInstance().CreateScene("Background Scene");

    const auto backgroundTexture = std::make_shared<GameObject>("BackgroundTexture");
    backgroundTexture->AddComponent<TextureComponent>(*backgroundTexture, "Textures/background.png");
    backgroundScene.Add(backgroundTexture);


    const auto backgroundLogoTexture = std::make_shared<GameObject>("Background Logo Texture");
    backgroundLogoTexture->AddComponent<TextureComponent>(*backgroundLogoTexture, "Textures/logo.png");
    auto backgroundLogotTextureComp = backgroundLogoTexture->GetComponent<TextureComponent>();

    const WindowSize windowSize = Renderer::GetInstance().GetSDLWindowSize();
    backgroundLogotTextureComp->m_bIsCenteredAtPosition = true;
    backgroundLogoTexture->SetWorldLocation({ static_cast<float>(windowSize.Width) / 2, static_cast<float>(windowSize.Height) / 2});
    backgroundScene.Add(backgroundLogoTexture);

    bae::Utils::DrawCircle({0, 0}, 1000, Utils::Color::Blue);
}

void LoadGameNameScene()
{
    auto& gameNameScene = SceneManager::GetInstance().CreateScene("Game Name Scene");
    auto font = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 48);

    const auto gameName = std::make_shared<GameObject>("Game Name");
    // gameName->AddComponent<TextComponent>(*gameName, "MsPacMan", font, bae::Utils::Color::Cyan);
    gameName->AddComponent<TextComponent>(*gameName, "MsPacMan", font, SDL_Color(255, 255, 0, 1));

    gameNameScene.Add(gameName);
}

void LoadFpsCounterScene()
{
    auto& fpsScene = SceneManager::GetInstance().CreateScene("FpsCounterScene");

    auto fontSmall = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);

    const auto fpsCounter = std::make_shared<GameObject>("Fps Counter");
    fpsCounter->AddComponent<FpsTextComponent>(*fpsCounter, fontSmall, SDL_Color(255, 255, 255, 255));

    const WindowSize windowSize = Renderer::GetInstance().GetSDLWindowSize();

    fpsCounter->SetWorldLocation({ windowSize.Width, 0.f });
    fpsCounter->AddLocation({ -75.f, 5.f });

    fpsScene.Add(fpsCounter);
}

void LoadRotatingObjectsScene()
{
    auto& keyboard = InputManager::GetInstance().GetKeyboard();

    auto& ballScene = SceneManager::GetInstance().CreateScene("Rotating Ball Scene");

    constexpr float moveSpeed = 800.f;
    const WindowSize windowSize = Renderer::GetInstance().GetSDLWindowSize();

    // LonelyRotatingBall
    const auto lonelyParent = std::make_shared<GameObject>("LonelyRotatingBall Parent");
    lonelyParent->SetWorldLocation({ static_cast<float>(windowSize.Width) / 2, static_cast<float>(windowSize.Height) / 2 });

    const auto lonelyRotatingBall = std::make_shared<GameObject>("LonelyRotatingBall");
    lonelyRotatingBall->AddComponent<bae::TextureComponent>(*lonelyRotatingBall, "Textures/SpriteExample.png");
    lonelyRotatingBall->AddComponent<Game::RotateComponent>(*lonelyRotatingBall, 100.f, 1.f);

    lonelyParent->AttachChild(lonelyRotatingBall.get());

    ballScene.Add(lonelyParent);
    ballScene.Add(lonelyRotatingBall);

    // Lonely Parent Commands
    auto moveUpLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(*lonelyParent, Game::Direction::Up, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveUpLonelyRotatingBallParentCommand), SDLK_W, InputManager::ButtonState::Pressed);

    auto moveDownLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(*lonelyParent, Game::Direction::Down, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveDownLonelyRotatingBallParentCommand), SDLK_S, InputManager::ButtonState::Pressed);

    auto moveLeftLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(*lonelyParent, Game::Direction::Left, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveLeftLonelyRotatingBallParentCommand), SDLK_A, InputManager::ButtonState::Pressed);

    auto moveRightLonelyRotatingBallParentCommand = std::make_unique<Game::MoveCommand>(*lonelyParent, Game::Direction::Right, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveRightLonelyRotatingBallParentCommand), SDLK_D, InputManager::ButtonState::Pressed);


    // Grouped Parent
    const auto groupedParent = std::make_shared<GameObject>("Grouped Main Parent");
    groupedParent->SetWorldLocation({ static_cast<float>(windowSize.Width) / 2 - 200.f, static_cast<float>(windowSize.Height) / 2 });
    ballScene.Add(groupedParent);

    // Grouped Parent Commands
    auto moveUpGroupedParentCommand = std::make_unique<Game::MoveCommand>(*groupedParent, Game::Direction::Up, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveUpGroupedParentCommand), SDLK_I, InputManager::ButtonState::Pressed);

    auto moveDownGroupedParentCommand = std::make_unique<Game::MoveCommand>(*groupedParent, Game::Direction::Down, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveDownGroupedParentCommand), SDLK_K, InputManager::ButtonState::Pressed);

    auto moveLeftGroupedParentCommand = std::make_unique<Game::MoveCommand>(*groupedParent, Game::Direction::Left, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveLeftGroupedParentCommand), SDLK_J, InputManager::ButtonState::Pressed);

    auto moveRightGroupedParentCommand = std::make_unique<Game::MoveCommand>(*groupedParent, Game::Direction::Right, moveSpeed);
    keyboard.AddKeyboardCommands(std::move(moveRightGroupedParentCommand), SDLK_L, InputManager::ButtonState::Pressed);

    // Grouped Rotating Ball
    const auto groupedRotatingBall = std::make_shared<GameObject>("Grouped Rotating Ball");
    groupedRotatingBall->AddComponent<bae::TextureComponent>(*groupedRotatingBall, "Textures/SpriteExample.png");
    groupedRotatingBall->AddComponent<Game::RotateComponent>(*groupedRotatingBall, 10.f, 5.f);

    groupedParent->AttachChild(groupedRotatingBall.get());

    ballScene.Add(groupedRotatingBall);

    // Grouped Rotating Child Ball
    const auto groupedRotatingChildBall = std::make_shared<GameObject>("Grouped Rotating Child Ball");
    groupedRotatingChildBall->AddComponent<bae::TextureComponent>(*groupedRotatingChildBall, "Textures/SpriteExample.png");
    groupedRotatingChildBall->AddComponent<Game::RotateComponent>(*groupedRotatingChildBall, 40.f, -2.f);

    groupedRotatingBall->AttachChild(groupedRotatingChildBall.get());

    ballScene.Add(groupedRotatingChildBall);
}

void PlayBeepSound()
{
    if (!MIX_Init())
    {
        std::cout << "Failed to initialize MIX: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to initialize MIX: " + std::string(SDL_GetError()));
    }

    const auto mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer)
    {
        std::cout << "Failed to make a MixerDevice Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to make a MixerDevice Error: " + std::string(SDL_GetError()));
    }

    const std::string beepSoundPath = std::filesystem::absolute(
        bae::ResourceManager::GetInstance().GetResourcesPath() / "Sounds/beep.mp3").string();

    const auto audio = MIX_LoadAudio(mixer, beepSoundPath.c_str(), false);
    if (!audio)
    {
        std::cout << beepSoundPath << std::endl;
        std::cout << "Failed to load Audio, Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to load Audio, Error: " + std::string(SDL_GetError()));
    }

    const auto track = MIX_CreateTrack(mixer);
    if (!track)
    {
        std::cout << "Failed to load Track, Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to load Track, Error: " + std::string(SDL_GetError()));
    }

    SDL_Delay(2000);
    MIX_SetTrackAudio(track, audio);
    MIX_PlayTrack(track, 0);
}


void PlayBeepSoundWithAudioStream()
{
    SDL_AudioSpec audioSpec
    {
        .format = SDL_AUDIO_F32,
        .channels = 2,
        .freq = 44100
    };

    Uint8* audioData = nullptr;
    Uint32 audioDataSize = 0;

    if (!SDL_Init(SDL_INIT_AUDIO))
    {
        std::cout << "Initialization Audio failed: " << SDL_GetError() << '\n';
        return;
    }

    const std::string beepWavSoundPath = std::filesystem::absolute(
        bae::ResourceManager::GetInstance().GetResourcesPath() / "Sounds/beep.wav").string();

    bool result = SDL_LoadWAV(beepWavSoundPath.c_str(), &audioSpec, &audioData, &audioDataSize);

    if (!result)
    {
        std::cout << "Failed to load Wav File (path: " << beepWavSoundPath << " ), Error: " << SDL_GetError() << '\n';
        return;
    }

    const auto audioStream = std::unique_ptr<SDL_AudioStream, Game::SDL_AudioStreamDeletor>(SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec, nullptr, nullptr));
    if (!audioStream)
    {
        std::cout << "Failed to create audio stream, Error: " << SDL_GetError() << '\n';
        return;
    }

    SDL_ResumeAudioStreamDevice(audioStream.get());

    result = SDL_PutAudioStreamData(audioStream.get(), audioData, static_cast<int>(audioDataSize));
    if (!result)
    {
        std::cout << "Failed to put data on audio's stream data, Error: " << SDL_GetError() << '\n';
        return;
    }

}
