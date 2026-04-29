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
#include <SDL3_mixer/SDL_mixer.h>

#include "Commands/TestSoundSystemCommands.hpp"
#include "Core/HelperFunctions.h"
#include "Core/ServiceLocator.h"
#include "Sounds/LoggingSoundSystem.h"
#include "Sounds/SdlAudioClip.h"
#include "Sounds/SdlSoundSystem.h"


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
#include "Base/SoundEvents.hpp"
#include "Commands/MoveCommand.hpp"
#include "Commands/MoveOnGridCommand.hpp"
#include "Components/RotateComponent.hpp"
#include "Components/TrashTheCacheComponent.hpp"


namespace fs = std::filesystem;

void Start();
void LoadBackground();
void LoadGameNameScene();
void LoadFpsCounterScene();
void LoadRotatingObjectsScene();
void LoadTrashTheCacheScene();

void LoadSounds();
void LoadTestSoundCommands();

void PlayBeepSound();
void PlayBeepSoundWithAudioStream();


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


    #if defined(_DEBUG) && __has_include(<vld.h>)
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
    SteamManager::GetInstance().Initialize();
    #endif

    #endif


    bae::BudgetEngine engine(window);
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
    // LoadTrashTheCacheScene();
    // PlayBeepSound();
    // PlayBeepSoundWithAudioStream();

    LoadSounds();
    LoadTestSoundCommands();
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
    groupedRotatingChildBall->AddComponent<Game::TrashTheCacheComponent>(*groupedRotatingChildBall, true);

    ballScene.Add(groupedRotatingChildBall);
}

void LoadTrashTheCacheScene()
{
    auto& trashCacheScene = bae::SceneManager::GetInstance().CreateScene("Trash The Cache");
    auto fontSmall        = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);

    const auto imguiObject = std::make_shared<bae::GameObject>("ImGui Object");
    imguiObject->AddComponent<Game::TrashTheCacheComponent>(*imguiObject, true);

    trashCacheScene.Add(imguiObject);
}


void LoadSounds()
{
    namespace gs = Game::Sounds;

    bae::ServiceLocator::RegisterAudioQueue<bae::SdlAudioClip>();
    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::SdlSoundSystem>());
    bae::ServiceLocator::RegisterSoundSystem(
        std::make_unique<bae::LoggingSoundSystem>(std::make_unique<bae::SdlSoundSystem>()));
    const auto soundSystem = &bae::ServiceLocator::GetSoundSystem();

    // Sound files not made yet
    gs::g_sSoundEvents =
    {
        { gs::SoundEvents::GameplayMusic, soundSystem->LoadSound("Resources/Sounds/beep.wav") },
        { gs::SoundEvents::PlayerDeath, soundSystem->LoadSound("Resources/Sounds/AsmrVoice.wav") },
    };
}


void LoadTestSoundCommands()
{
    namespace gs = Game::Sounds;

    auto& keyboard   = bae::InputManager::GetInstance().GetKeyboard();
    auto soundSystem = &bae::ServiceLocator::GetSoundSystem();


    // Play & Pause Immediately after
    auto activeSoundId = soundSystem->Play(gs::GetSoundId(gs::SoundEvents::PlayerDeath), 1);
    soundSystem->Pause(activeSoundId);


    // Sound Commands:
    std::unique_ptr<gs::TestPlaySoundCommand> soundPlayCommands{};
    std::unique_ptr<gs::TestSoundSystemCommand> soundCommands{};

    // Play
    soundPlayCommands = std::make_unique<gs::TestPlaySoundCommand>(gs::GetSoundId(gs::SoundEvents::PlayerDeath));
    keyboard.AddKeyboardCommands(std::move(soundPlayCommands), SDLK_1, bae::InputManager::ButtonState::Down);

    // TogglePause
    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::TogglePause, activeSoundId);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_2, bae::InputManager::ButtonState::Down);

    // ToggleMute
    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::ToggleMute, activeSoundId);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_3, bae::InputManager::ButtonState::Down);


    // Change Volume
    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::SetVolume, activeSoundId, 0.5f);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_4, bae::InputManager::ButtonState::Down);

    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::SetVolume, activeSoundId, 1.0f);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_5, bae::InputManager::ButtonState::Down);


    // All Sounds Commands

    // Change Volume
    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::SetVolumeAll,
                                                                 bae::ActiveSoundID(-1), 0.2f);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_6, bae::InputManager::ButtonState::Down);

    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::SetVolumeAll,
                                                                 bae::ActiveSoundID(-1), 0.7f);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_7, bae::InputManager::ButtonState::Down);

    // StopAll
    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::StopAll);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_8, bae::InputManager::ButtonState::Down);

    // TogglePauseAll/ToglleMuteAll
    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::TogglePauseAll);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_9, bae::InputManager::ButtonState::Down);

    soundCommands = std::make_unique<gs::TestSoundSystemCommand>(gs::TestSoundEvents::ToggleMuteAll);
    keyboard.AddKeyboardCommands(std::move(soundCommands), SDLK_0, bae::InputManager::ButtonState::Down);
}


void PlayBeepSound()
{
    if(!MIX_Init())
    {
        std::cout << "Failed to initialize MIX: " << SDL_GetError() << '\n';
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed to initialize MIX: ") + SDL_GetError());
    }

    const auto mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if(!mixer)
    {
        std::cout << "Failed to make a MixerDevice Error: " << SDL_GetError() << '\n';
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed to make a MixerDevice Error: ") + SDL_GetError());
    }

    const std::string beepSoundPath = std::filesystem::absolute(
        bae::ResourceManager::GetInstance().GetResourcesPath() / "Sounds/beep.mp3").string();

    const auto audio = MIX_LoadAudio(mixer, beepSoundPath.c_str(), false);
    if(!audio)
    {
        std::cout << beepSoundPath << '\n';
        std::cout << "Failed to load Audio, Error: " << SDL_GetError() << '\n';
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed to load Audio, Error: ") + SDL_GetError());
    }

    const auto track = MIX_CreateTrack(mixer);
    if(!track)
    {
        std::cout << "Failed to load Track, Error: " << SDL_GetError() << '\n';
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed to load Track, Error: ") + SDL_GetError());
    }

    SDL_Delay(2000);
    MIX_SetTrackAudio(track, audio);
    MIX_PlayTrack(track, 0);
}


void PlayBeepSoundWithAudioStream()
{
    SDL_AudioSpec audioSpec
    {
        .format   = SDL_AUDIO_F32,
        .channels = 2,
        .freq     = 44100
    };

    Uint8* audioData     = nullptr;
    Uint32 audioDataSize = 0;

    if(!SDL_Init(SDL_INIT_AUDIO))
    {
        std::cout << "Initialization Audio failed: " << SDL_GetError() << '\n';
        return;
    }

    const std::string beepWavSoundPath = std::filesystem::absolute(
        bae::ResourceManager::GetInstance().GetResourcesPath() / "Sounds/beep.wav").string();

    bool result = SDL_LoadWAV(beepWavSoundPath.c_str(), &audioSpec, &audioData, &audioDataSize);

    if(!result)
    {
        std::cout << "Failed to load Wav File (path: " << beepWavSoundPath << " ), Error: " << SDL_GetError() << '\n';
        return;
    }

    const auto audioStream = std::unique_ptr<SDL_AudioStream, Game::SDL_AudioStreamDeletor>(
        SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec, nullptr, nullptr));
    if(!audioStream)
    {
        std::cout << "Failed to create audio stream, Error: " << SDL_GetError() << '\n';
        return;
    }

    SDL_ResumeAudioStreamDevice(audioStream.get());

    result = SDL_PutAudioStreamData(audioStream.get(), audioData, static_cast<int>(audioDataSize));
    if(!result)
    {
        std::cout << "Failed to put data on audio's stream data, Error: " << SDL_GetError() << '\n';
    }
}
