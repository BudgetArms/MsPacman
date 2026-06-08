#include "LevelManagerComponent.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"
#include "Wrappers/Keyboard.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Commands/MoveCommand.hpp"
#include "Commands/MoveOnGridCommand.hpp"
#include "Commands/TestDamageCommand.hpp"
#include "Commands/TestScoreCommand.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LevelGridComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/LifeDisplayComponent.hpp"
#include "Components/MsPacmanComponent.hpp"
#include "Components/RenderCenterComponent.hpp"
#include "Components/ScoreComponent.hpp"
#include "Components/ScoreDisplayComponent.hpp"
#include "Components/TextureComponent.hpp"


using namespace Game;


LevelManagerComponent::LevelManagerComponent(bae::GameObject& owner, const GameMode gameMode) :
    Component(owner),
    m_GameMode{ gameMode }
{
    // Create Background
    m_BackgroundSpriteSheet = std::make_unique<bae::SpriteSheet>(m_BackgroundTexturePath,
                                                                 SDL_FRect(0, 0, 224, 1488), 1, 6);
}

LevelManagerComponent::~LevelManagerComponent()
{
    ClearLevel();
}

void LevelManagerComponent::Render() const
{
    m_BackgroundSpriteSheet->Render();
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

void LevelManagerComponent::Notify(const unsigned int eventHash, bae::Subject*, const std::any&)
{
    switch(GetEvent(eventHash))
    {
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
        case Events::BeginLevel:
        case Events::RestartLevel:
        case Events::ScoreChanged:
        case Events::LivesChanged:
        case Events::InvincibilityChanged:
        case Events::NoEvent:
        case Events::Collision:
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
        / m_LevelRepeatTimes) % static_cast<int>(m_LevelJson.size());

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


    const LevelJson& levelJson = currentLevelJson.value();

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
    m_LevelGridComponent = levelGrid->GetComponent<LevelGridComponent>();

    m_LevelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 0 });
    m_LevelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 1 });
    m_LevelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 2 });

    m_LevelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 0, 0 });
    m_LevelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 1, 0 });
    m_LevelGridComponent->RemoveNode(bae::Graphs::GridPosition{ 2, 0 });

    for(const bae::Graphs::GridPosition& gridPosition : levelJson.NodesToRemove)
    {
        m_LevelGridComponent->RemoveNode(gridPosition);
    }

    for(const bae::Graphs::GridPosition& gridPosition : levelJson.NodesToAddAfterRemoval)
    {
        m_LevelGridComponent->AddNode(gridPosition);
        m_LevelGridComponent->AddConnectionsToNeighbors(gridPosition);
    }


    // m_LevelGridComponent->SetRenderCells(true);
    // m_LevelGridComponent->SetRenderNodes(true);
    m_LevelGridComponent->SetRenderConnections(true);

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
        case Events::LevelLost:
        case Events::GhostDied:
            break;
        case Events::BeginLevel:
            ClearLevel();
            CreateLevel();
            break;
        case Events::RestartLevel:
        case Events::ScoreChanged:
        case Events::LivesChanged:
        case Events::InvincibilityChanged:
        case Events::NoEvent:
        case Events::Collision:
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
    auto msPacmanComp = msPacman->GetComponent<MsPacmanComponent>();

    constexpr glm::vec2 dimensions = { 50, 50 };
    constexpr glm::vec2 offset     = { -dimensions.x / 2.f, -dimensions.y / 2.f };

    msPacman->AddComponent<HitboxComponent>(*msPacman, dimensions, offset);
    const auto hitboxComp = msPacman->GetComponent<HitboxComponent>();
    hitboxComp->SetVisibility(true);

    hitboxComp->AddObserver(msPacmanComp);

    // Score Display
    auto text = std::make_unique<bae::Text2D>("Score: Test");
    msPacman->AddComponent<ScoreDisplayComponent>(*msPacman, glm::vec2{ 100, 550 }, std::move(text));

    const auto scoreDisplayComp = msPacman->GetComponent<ScoreDisplayComponent>();
    msPacman->GetComponent<ScoreComponent>()->AddObserver(scoreDisplayComp);

    // Life Display
    auto lifeTexture = bae::ResourceManager::GetInstance().LoadTexture("Textures/Popup/MsPacmanLife.png");
    msPacman->AddComponent<LifeDisplayComponent>(*msPacman, glm::vec2{ 100, 400 }, lifeTexture);

    const auto lifeDisplayComp = msPacman->GetComponent<LifeDisplayComponent>();
    msPacman->GetComponent<LifeComponent>()->AddObserver(lifeDisplayComp);


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

    msPacman->AddComponent<GridMovementComponent>(*msPacman, *m_LevelGridComponent);

    // MoveOnGridCommand
    auto moveOnGridLeftCommand  = std::make_unique<MoveOnGridCommand>(*msPacman, Direction::Left);
    auto moveOnGridRightCommand = std::make_unique<MoveOnGridCommand>(*msPacman, Direction::Right);
    auto moveOnGridDownCommand  = std::make_unique<MoveOnGridCommand>(*msPacman, Direction::Down);
    auto moveOnGridUpCommand    = std::make_unique<MoveOnGridCommand>(*msPacman, Direction::Up);

    constexpr bae::InputManager::ButtonState moveOnGridButtonState = bae::InputManager::ButtonState::Down;
    keyboard.AddKeyboardCommands(std::move(moveOnGridLeftCommand), SDLK_LEFT, moveOnGridButtonState);
    keyboard.AddKeyboardCommands(std::move(moveOnGridRightCommand), SDLK_RIGHT, moveOnGridButtonState);
    keyboard.AddKeyboardCommands(std::move(moveOnGridDownCommand), SDLK_DOWN, moveOnGridButtonState);
    keyboard.AddKeyboardCommands(std::move(moveOnGridUpCommand), SDLK_UP, moveOnGridButtonState);


    // TODO: remove both
    // Test Damage Command (will be removed after testing)
    auto damageCommand = std::make_unique<TestDamageCommand>(*msPacman);
    keyboard.AddKeyboardCommands(std::move(damageCommand), SDLK_V, bae::InputManager::ButtonState::Down);

    // Test Score Command (will be removed after testing)
    auto scoreCommand = std::make_unique<TestScoreCommand>(*msPacman);
    keyboard.AddKeyboardCommands(std::move(scoreCommand), SDLK_B, bae::InputManager::ButtonState::Down);


    scene->Add(msPacman);


    // TODO: remove after testing
    const auto enemyObject = std::make_shared<bae::GameObject>("Enemy Collision");
    enemyObject->SetWorldLocation({ 200, 200 });

    enemyObject->AddComponent<bae::TextureComponent>(*enemyObject, "Textures/Characters/Blinky.png");
    enemyObject->AddComponent<HitboxComponent>(*enemyObject, glm::vec2{ 20, 20 }, glm::vec2{ 0, 0 });
    auto testingHitboxComp = enemyObject->GetComponent<HitboxComponent>();
    testingHitboxComp->SetVisibility(true);

    scene->Add(enemyObject);
}



