#include "LevelManagerComponent.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "HitboxComponent.hpp"
#include "LevelGridComponent.hpp"
#include "MsPacmanComponent.hpp"
#include "RenderCenterComponent.hpp"
#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Commands/MoveCommand.hpp"
#include "Commands/TestDamageCommand.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"
#include "Wrappers/Keyboard.hpp"


using namespace Game;


LevelManagerComponent::LevelManagerComponent(bae::GameObject& owner, const GameMode gameMode) :
    Component(owner),
    m_GameMode{ gameMode }
{
    // Create Background
    m_BackgroundSpriteSheet = std::make_unique<bae::SpriteSheet>(m_BackgroundTexturePath,
                                                                 SDL_FRect(0, 0, 224, 1488), 1, 6);

    bae::SceneManager::GetInstance().CreateScene(g_LevelSceneName.data());
}

LevelManagerComponent::~LevelManagerComponent()
{
    ClearLevel();
}

void LevelManagerComponent::Render() const
{
    m_BackgroundSpriteSheet->Render();
    bae::Utils::FillCircle(m_BackgroundSpriteSheet->m_Position, 10, bae::Utils::Color::Green);
}

void LevelManagerComponent::LoadLevelFromFile(int levelNumber, const std::filesystem::path& jsonFile)
{
    if(levelNumber < 0)
    {
        std::cout << FUNCTION_NAME << " Failed To Set LevelNumber! LevelNumber Cannot Be Negative!" << '\n';
        return;
    }

    const auto resourceFolder = bae::ResourceManager::GetInstance().GetResourcesPath();
    if(!std::filesystem::exists(resourceFolder / jsonFile))
    {
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed! File Not found! file") + jsonFile.string());
    }

    std::ifstream file{ resourceFolder / jsonFile, std::ios::binary };
    if(!file.is_open())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Open File: ") + jsonFile.string());
    }

    nlohmann::json json{};
    LevelJson levelJson{};

    try
    {
        file >> json;
        levelJson = json.get<LevelJson>();
    }
    catch(const std::exception& exception)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Parse Json File! Exception: ")
            + std::string(exception.what()));
    }

    m_LevelJson.insert({ levelNumber, levelJson });
}

void LevelManagerComponent::CreateLevel()
{
    const std::optional<LevelJson> currentLevelJson = GetCurrentLevelJson();
    if(!currentLevelJson.has_value())
    {
        return;
    }

    const LevelJson levelJson = currentLevelJson.value();

    CreateGrid();

    AddPlayers();
}

void LevelManagerComponent::ResetLevel()
{
    ClearLevel();
    CreateLevel();
}


int LevelManagerComponent::GetCurrentLevel() const
{
    return m_CurrentLevel;
}

void LevelManagerComponent::Notify(const unsigned int eventHash, bae::Subject*)
{
    switch(GetEvent(eventHash))
    {
        // case Events::GamePaused:
        // break;
        // case Events::GameResumed:
        // break;
        case Events::PlayerDied:
            break;
        case Events::GameOver:
            ResetLevel();
            break;
        case Events::LevelWon:
            WonLevel();
            break;
        case Events::LevelLost:
            LostLevel();
            break;
        case Events::GhostDied:
            break;
        case Events::BeginLevel:
            break;
        case Events::RestartLevel:
            break;
        case Events::ScoreChanged:
            break;
        case Events::LivesChanged:
            break;
        case Events::InvincibilityChanged:
            break;
        case Events::NoEvent:
            break;
    }
}

void LevelManagerComponent::SetSpriteSheetWorldLocation(const glm::vec2& location) const
{
    m_BackgroundSpriteSheet->m_Position = location;
}

void LevelManagerComponent::SetSpriteSheetWorldRotation(const float rotation) const
{
    m_BackgroundSpriteSheet->m_Rotation = rotation;
}


void LevelManagerComponent::SetSpriteSheetWorldScale(const glm::vec2& scale) const
{
    m_BackgroundSpriteSheet->m_Scale = scale;
}


void LevelManagerComponent::WonLevel()
{
    ++m_CurrentLevel;
    ClearLevel();
    CreateLevel();
}

void LevelManagerComponent::LostLevel()
{
    ResetLevel();
}

void LevelManagerComponent::LoadBackground(const int levelIndex) const
{
    if(m_LevelJson.contains(levelIndex))
    {
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed! LevelJson Not Found! Index: " + std::to_string(levelIndex)));
    }

    m_BackgroundSpriteSheet->m_Index = levelIndex;
}

void LevelManagerComponent::ClearLevel() const
{
    // To avoid a deletion of objects if the scene is already getting deleted
    if(m_Owner->IsMarkedForDeletion())
    {
        return;
    }

    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());
    if(scene)
    {
        scene->ForceRemoveAll();
    }
}

std::optional<LevelJson> LevelManagerComponent::GetCurrentLevelJson()
{
    if(m_LevelJson.empty())
    {
        return std::nullopt;
    }

    const int currentLevelIndex = static_cast<int>(static_cast<float>(m_CurrentLevel)
        / m_LevelRepeatTimes) % m_LevelJson.size();

    const auto levelJsonIt = m_LevelJson.find(currentLevelIndex);
    if(levelJsonIt == m_LevelJson.end())
    {
        return std::nullopt;
    }

    return levelJsonIt->second;
}

void LevelManagerComponent::CreateGrid()
{
    const std::optional<LevelJson> currentLevelJson = GetCurrentLevelJson();
    if(!currentLevelJson.has_value())
    {
        return;
    }


    const LevelJson levelJson = currentLevelJson.value();

    const bae::WindowSize gridDimensions = levelJson.Dimensions;
    const glm::vec2 gridSize             = { gridDimensions.Width, gridDimensions.Height };
    const int nrColumns                  = levelJson.NrColumns;
    const int nrRows                     = levelJson.NrRows;

    bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());
    if(!scene)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Get LevelScene!"));
    }

    const auto levelGrid             = std::make_shared<bae::GameObject>("LevelGrid");
    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    // gridSize
    levelGrid->SetWorldLocation({ -gridSize.x / 2.f, -gridSize.y / 2.f });
    levelGrid->AddLocation({ static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f });

    levelGrid->AddComponent<LevelGridComponent>(*levelGrid, gridSize, nrColumns, nrRows);
    [[maybe_unused]] auto const levelGridComponent = levelGrid->GetComponent<Game::LevelGridComponent>();

    levelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 0 });
    levelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 1 });
    levelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 2 });

    levelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 0 });
    levelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 1, 0 });
    levelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 2, 0 });

    for(const bae::Graphs::GridPosition& gridPosition : levelJson.NodesToRemove)
    {
        levelGridComponent->RemoveNode(gridPosition);
    }

    for(const bae::Graphs::GridPosition& gridPosition : levelJson.NodesToAddAfterRemoval)
    {
        levelGridComponent->AddNode(gridPosition);
        levelGridComponent->AddConnectionsToNeighbors(gridPosition);
    }


    // levelGridComponent->SetRenderCells(true);
    // levelGridComponent->SetRenderNodes(true);
    levelGridComponent->SetRenderConnections(true);

    scene->Add(levelGrid);
}

void LevelManagerComponent::HandleEvent(const unsigned int eventHash)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
            HandlePlayerDied();
            break;
        case Events::GameOver:
        case Events::LevelWon:
            break;
        case Events::LevelLost:
            break;
        case Events::GhostDied:
            break;
        case Events::BeginLevel:
            ClearLevel();
            CreateLevel();
            break;
        case Events::RestartLevel:
            break;
        case Events::ScoreChanged:
        case Events::LivesChanged:
        case Events::InvincibilityChanged:
        case Events::NoEvent:
            break;
    }
}

void LevelManagerComponent::HandlePlayerDied() const
{
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            std::cout << FUNCTION_NAME << " Singleplayer\n";
            break;
        case GameMode::CoOp:
            std::cout << FUNCTION_NAME << " Co-Op\n";
            break;
        case GameMode::Versus:
            std::cout << FUNCTION_NAME << " Versus\n";
            break;
    }
}

void LevelManagerComponent::AddPlayers() const
{
    // Player One & Controls
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto msPacman = std::make_shared<bae::GameObject>("MsPacman");

    msPacman->AddComponent<MsPacmanComponent>(*msPacman);

    constexpr glm::vec2 dimensions = { 50, 50 };
    constexpr glm::vec2 offset     = { -dimensions.x / 2.f, -dimensions.y / 2.f };

    msPacman->AddComponent<HitboxComponent>(*msPacman, dimensions, offset);
    const auto hitboxComp = msPacman->GetComponent<HitboxComponent>();
    hitboxComp->SetVisibility(true);


    // // Controls
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


    // Test Damage Command (will be removed after testing)
    auto damageCommand = std::make_unique<Game::TestDamageCommand>(*msPacman);
    keyboard.AddKeyboardCommands(std::move(damageCommand), SDLK_V, bae::InputManager::ButtonState::Down);


    scene->Add(msPacman);
}



