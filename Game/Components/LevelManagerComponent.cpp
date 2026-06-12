#include "LevelManagerComponent.hpp"

#if WIN32
#include <Windows.h>
#include <XInput.h>
#endif

#include <fstream>

#include <nlohmann/json.hpp>

#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Components/SpriteComponent.hpp"
#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"
#include "Wrappers/Keyboard.hpp"

#include "Components/BlinkyComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Commands/MoveCommand.hpp"
#include "Commands/MoveOnGridCommand.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LevelGridComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/LifeDisplayComponent.hpp"
#include "Components/MsPacmanComponent.hpp"
#include "Components/RenderCenterComponent.hpp"
#include "Components/ScoreComponent.hpp"
#include "Components/ScoreDisplayComponent.hpp"
#include "Wrappers/Controller.hpp"


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
    AddGhosts();
    AddItems();
}

void LevelManagerComponent::ResetLevel()
{
    ClearLevel();
    CreateLevel();
}

LevelGridComponent* LevelManagerComponent::GetLevelGridComponent() const
{
    return m_LevelGridComponent;
}


int LevelManagerComponent::GetCurrentLevelNumber() const
{
    return m_CurrentLevel;
}

LevelJson LevelManagerComponent::GetCurrentLevel()
{
    return GetCurrentLevelJson().value();
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
        case Events::DirectionChanged:
            break;
        case Events::GameOver:
            ClearLevel();
            break;
        case Events::LevelWon:
            WonLevel();
        case Events::LevelLost:
            LostLevel();
        case Events::GhostDied:
            break;
        case Events::BeginLevel:
            ClearLevel();
            CreateLevel();
            break;
        case Events::RestartLevel:
        case Events::ScoreChanged:
        case Events::LivesChanged:
            ResetLevel();
            break;
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
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            SpawnMsPacman();
            break;
        case GameMode::CoOp:
            SpawnMsPacman();
            SpawnMrPacman();
            break;
        case GameMode::Versus:
            SpawnMsPacman();
            break;
    }
}

void LevelManagerComponent::AddGhosts() const
{
    SpawnBlinky();
}

void LevelManagerComponent::AddItems() const
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());


    const auto fruitObject = std::make_shared<bae::GameObject>("Fruit Item");
    fruitObject->SetWorldLocation(m_LevelGridComponent->GetPosition({ 24, 22 }));

    fruitObject->AddComponent<ItemComponent>(*fruitObject, ItemType::Fruit);
    fruitObject->AddComponent<HitboxComponent>(*fruitObject, glm::vec2{ 20, 20 }, glm::vec2{ 0, 0 });

    fruitObject->GetComponent<HitboxComponent>()->SetVisibility(true);

    scene->Add(fruitObject);


    const auto powerPellet = std::make_shared<bae::GameObject>("PowerPellet Item");
    powerPellet->SetWorldLocation(m_LevelGridComponent->GetPosition({ 20, 26 }));

    powerPellet->AddComponent<ItemComponent>(*powerPellet, ItemType::PowerPellet);
    powerPellet->AddComponent<HitboxComponent>(*powerPellet, glm::vec2{ 20, 20 }, glm::vec2{ 0, 0 });
    powerPellet->GetComponent<HitboxComponent>()->SetVisibility(true);

    scene->Add(powerPellet);


    /*
    const auto powerPellet2 = std::make_shared<bae::GameObject>("PowerPellet Item");
    powerPellet->SetWorldLocation(m_LevelGridComponent->GetPosition({ 5, 26 }));

    powerPellet->AddComponent<ItemComponent>(*powerPellet, ItemType::PowerPellet);
    powerPellet->AddComponent<HitboxComponent>(*powerPellet, glm::vec2{ 20, 20 }, glm::vec2{ 0, 0 });
    powerPellet->GetComponent<HitboxComponent>()->SetVisibility(true);

    */
}

std::shared_ptr<bae::GameObject> LevelManagerComponent::GetGhostBase(const std::string& gameObjectName,
                                                                     const glm::vec2& spawnPosition)
{
    const auto ghost = std::make_shared<bae::GameObject>(gameObjectName);
    ghost->SetWorldLocation(spawnPosition);

    constexpr glm::vec2 dimensions = { 20, 20 };
    constexpr glm::vec2 offset     = { -dimensions.x / 2.f, -dimensions.y / 2.f };

    ghost->AddComponent<HitboxComponent>(*ghost, dimensions, offset);
    ghost->GetComponent<HitboxComponent>()->SetVisibility(true);

    return ghost;
}

void LevelManagerComponent::SpawnBlinky() const
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = m_LevelGridComponent->GetPosition({ 13, 11 });

    const auto blinky = GetGhostBase("Ghost Blinky", spawnPosition);

    blinky->AddComponent<BlinkyComponent>(*blinky, m_LevelGridComponent, spawnPosition);

    const auto blinkyComp = blinky->GetComponent<BlinkyComponent>();
    blinky->GetComponent<GridMovementComponent>()->AddObserver(blinkyComp);

    if(m_GameMode == GameMode::Versus)
    {
        AddControls(*blinky.get(), false);
    }

    scene->Add(blinky);
}

std::shared_ptr<bae::GameObject> LevelManagerComponent::GetMsPacmanBase(const std::string& gameObjectName,
                                                                        const glm::vec2& spawnPosition) const
{
    const auto msPacman = std::make_shared<bae::GameObject>(gameObjectName);
    msPacman->SetWorldLocation(spawnPosition);

    msPacman->AddComponent<MsPacmanComponent>(*msPacman, m_LevelGridComponent);
    const auto msPacmanComp = msPacman->GetComponent<MsPacmanComponent>();

    constexpr glm::vec2 dimensions = { 20, 20 };
    constexpr glm::vec2 offset     = { -dimensions.x / 2.f, -dimensions.y / 2.f };

    msPacman->AddComponent<HitboxComponent>(*msPacman, dimensions, offset);
    msPacman->GetComponent<HitboxComponent>()->SetVisibility(true);
    msPacman->GetComponent<HitboxComponent>()->AddObserver(msPacmanComp);

    // Score Display
    auto text = std::make_unique<bae::Text2D>("Score: 0");
    msPacman->AddComponent<ScoreDisplayComponent>(*msPacman, glm::vec2{ 0, 0 }, std::move(text));

    const auto scoreDisplayComp = msPacman->GetComponent<ScoreDisplayComponent>();
    msPacman->GetComponent<ScoreComponent>()->AddObserver(scoreDisplayComp);

    // Life Display
    auto lifeTexture = bae::ResourceManager::GetInstance().LoadTexture("Textures/Popup/MsPacmanLife.png");
    msPacman->AddComponent<LifeDisplayComponent>(*msPacman, glm::vec2{ 0, 0 }, lifeTexture);
    msPacman->GetComponent<LifeDisplayComponent>()->m_Scale = { 2.f, 2.f };

    const auto lifeDisplayComp = msPacman->GetComponent<LifeDisplayComponent>();
    msPacman->GetComponent<LifeComponent>()->AddObserver(lifeDisplayComp);
    msPacman->GetComponent<GridMovementComponent>()->m_Speed = 100.f;
    msPacman->GetComponent<GridMovementComponent>()->AddObserver(msPacmanComp);

    return msPacman;
}


void LevelManagerComponent::SpawnMsPacman() const
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = m_LevelGridComponent->GetPosition({ 3, 3 });

    const auto msPacman = GetMsPacmanBase("MsPacman", spawnPosition);

    // Score Display
    msPacman->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 5, 540 });

    // Life Display
    msPacman->GetComponent<LifeDisplayComponent>()->m_Position = { 160, 540 };

    // Controls
    AddControls(*msPacman, true);

    scene->Add(msPacman);
}

void LevelManagerComponent::SpawnMrPacman() const
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = m_LevelGridComponent->GetPosition({ 25, 1 });

    const auto mrPacman = GetMsPacmanBase("MsPacman", spawnPosition);


    mrPacman->GetComponent<bae::SpriteComponent>()->SetTexture("Textures/Characters/Pacman.png");

    // Score Display
    mrPacman->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 750, 540 });

    // Life Display
    const auto lifeTexture = bae::ResourceManager::GetInstance().LoadTexture("Textures/Popup/MrPacmanLife.png");
    mrPacman->GetComponent<LifeDisplayComponent>()->SetTexture(lifeTexture);
    mrPacman->GetComponent<LifeDisplayComponent>()->m_Position = { 900, 540 };

    // Controls
    AddControls(*mrPacman, false);

    scene->Add(mrPacman);
}

void LevelManagerComponent::AddControls(bae::GameObject& gameObject, const bool firstPlayer)
{
    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    constexpr auto moveOnGridButtonState = bae::InputManager::ButtonState::Pressed;

    if(firstPlayer)
    {
        auto moveOnGridLeftCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Left);
        auto moveOnGridRightCommand = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Right);
        auto moveOnGridDownCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Down);
        auto moveOnGridUpCommand    = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Up);

        keyboard.AddKeyboardCommands(std::move(moveOnGridLeftCommand), SDLK_A, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(moveOnGridRightCommand), SDLK_D, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(moveOnGridDownCommand), SDLK_S, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(moveOnGridUpCommand), SDLK_W, moveOnGridButtonState);
    }

    if(!firstPlayer)
    {
        auto moveOnGridLeftCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Left);
        auto moveOnGridRightCommand = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Right);
        auto moveOnGridDownCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Down);
        auto moveOnGridUpCommand    = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Up);

        keyboard.AddKeyboardCommands(std::move(moveOnGridLeftCommand), SDLK_LEFT, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(moveOnGridRightCommand), SDLK_RIGHT, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(moveOnGridDownCommand), SDLK_DOWN, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(moveOnGridUpCommand), SDLK_UP, moveOnGridButtonState);
    }


    const bae::Controller* controller = bae::InputManager::GetInstance().GetController(!firstPlayer);

    if(!controller)
    {
        std::cout << FUNCTION_NAME << " Failed to Get controller, is first player: " << firstPlayer << '\n';
        return;
    }

    [[maybe_unused]] auto moveOnGridLeftCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Left);
    [[maybe_unused]] auto moveOnGridRightCommand = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Right);
    [[maybe_unused]] auto moveOnGridDownCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Down);
    [[maybe_unused]] auto moveOnGridUpCommand    = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Up);

    #if WIN32
    controller->AddControllerCommands(std::move(moveOnGridLeftCommand), XINPUT_GAMEPAD_DPAD_LEFT,
                                      moveOnGridButtonState);
    controller->AddControllerCommands(std::move(moveOnGridRightCommand), XINPUT_GAMEPAD_DPAD_RIGHT,
                                      moveOnGridButtonState);
    controller->AddControllerCommands(std::move(moveOnGridDownCommand), XINPUT_GAMEPAD_DPAD_DOWN,
                                      moveOnGridButtonState);
    controller->AddControllerCommands(std::move(moveOnGridUpCommand), XINPUT_GAMEPAD_DPAD_UP, moveOnGridButtonState);
    #endif
}



