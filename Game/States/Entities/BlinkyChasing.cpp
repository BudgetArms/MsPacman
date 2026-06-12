#include "BlinkyChasing.hpp"

#include "BlinkyFrightened.hpp"
#include "Components/SpriteComponent.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"
#include "Singletons/GameTime.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Components/LevelGridComponent.hpp"
#include "Components/MsPacmanComponent.hpp"


using namespace Game;


States::BlinkyChasing::BlinkyChasing(bae::GameObject& gameObject) :
    GhostStates(gameObject),
    m_Movement{ gameObject, *m_GridMovementComponent, 50.f }
{
}

void States::BlinkyChasing::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    auto spriteComponent = m_GameObject->GetComponent<bae::SpriteComponent>();

    spriteComponent->SetTexture(m_BlinkySpritePath.data());
    spriteComponent->SetSrcRect(m_BlinkySrcRect);
    spriteComponent->SetColumns(m_NrColumnsSprite);
    spriteComponent->SetSprites(m_BlinkyNrSprites);

    spriteComponent->m_Index = m_BlinkySpriteIndexOffset +
            m_NrColumnsSprite * static_cast<int>(m_GridMovementComponent->GetDirection());
}

void States::BlinkyChasing::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

std::unique_ptr<States::EntityState> States::BlinkyChasing::Update()
{
    UpdateTargetPosition();

    m_Movement.SetTargetPosition(m_TargetPosition);
    m_Movement.FixedUpdate();

    if(m_bIsFrightened)
    {
        return std::make_unique<BlinkyFrightened>(*m_GameObject, GetRandomCornerPosition());
    }

    return nullptr;
}


void States::BlinkyChasing::UpdateTargetPosition()
{
    const auto players = GetPlayersGameObjects();
    if(players.empty())
    {
        return;
    }

    const glm::vec2 position = m_GameObject->GetWorldLocation();
    float closestDistance    = FLT_MAX;

    for(const auto& object : players)
    {
        const glm::vec2 playerPosition = object->GetWorldLocation();
        const float newDistance        = glm::distance(position, playerPosition);

        if(newDistance <= closestDistance)
        {
            closestDistance  = newDistance;
            m_TargetPosition = playerPosition;
        }
    }
}

std::vector<bae::GameObject*> States::BlinkyChasing::GetPlayersGameObjects()
{
    bae::Scene* levelScene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    std::vector<bae::GameObject*> players{};

    for(const auto& object : levelScene->GetObjects())
    {
        if(object->HasComponent<MsPacmanComponent>())
        {
            players.push_back(object.get());
        }
    }

    return players;
}

void States::BlinkyChasing::HandleEvent(const unsigned eventHash)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
        case Events::DirectionChanged:
        case Events::GameOver:
        case Events::LevelWon:
        case Events::LevelLost:
        case Events::GhostDied:
        case Events::BeginLevel:
        case Events::RestartLevel:
        case Events::ScoreChanged:
        case Events::LivesChanged:
            break;
        case Events::InvincibilityChanged:
        {
            m_bIsFrightened = true;
        }
        break;
        case Events::Collision:
        case Events::NoEvent:
            break;
    }
}

glm::vec2 States::BlinkyChasing::GetRandomCornerPosition() const
{
    auto isEvenFunc = [](const int n) -> bool
    {
        return n % 2 == 0;
    };


    if(isEvenFunc(bae::GetRandomNumber()))
    {
        // Left Corner
        if(isEvenFunc(bae::GetRandomNumber()))
        {
            // Left Up Corner
            return m_GridMovementComponent->GetLevelGridComponent()->GetPosition({ 1, 1 });
        }

        // Left Down Corner
        return m_GridMovementComponent->GetLevelGridComponent()->GetPosition({ 1, 28 });
    }

    // Right Corner
    if(isEvenFunc(bae::GetRandomNumber()))
    {
        // Right Up Corner
        return m_GridMovementComponent->GetLevelGridComponent()->GetPosition({ 25, 1 });
    }

    // Right Down Corner
    return m_GridMovementComponent->GetLevelGridComponent()->GetPosition({ 25, 28 });
}
