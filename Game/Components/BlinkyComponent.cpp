#include "BlinkyComponent.hpp"

#include "Components/SpriteComponent.hpp"

#include "Base/Events.hpp"
#include "Components/GridMovementComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LevelGridComponent.hpp"
#include "States/Entities/BlinkyChasing.hpp"
#include "States/Entities/BlinkyEaten.hpp"
#include "States/Entities/GhostStates.hpp"


using namespace Game;


BlinkyComponent::BlinkyComponent(bae::GameObject& owner, LevelGridComponent* levelGridComp,
                                 const glm::vec2& spawnPosition) :
    Component(owner),
    m_SpawnPosition{ spawnPosition }
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_BlinkySpritePath.data(),
                                                SDL_FRect(0, 0, 32, 64), 2, 8);
    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();

    m_Owner->AddComponent<GridMovementComponent>(*m_Owner, *levelGridComp);
    m_GridMovementComponent = m_Owner->GetComponent<GridMovementComponent>();

    m_BlinkyState = std::make_unique<States::BlinkyChasing>(*m_Owner);
    m_BlinkyState->OnEnter();
}

BlinkyComponent::~BlinkyComponent() = default;

void BlinkyComponent::FixedUpdate()
{
    UpdateToNewState(m_BlinkyState->Update());
}

States::EntityState* BlinkyComponent::GetState() const
{
    return m_BlinkyState.get();
}

void BlinkyComponent::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
        case Events::DirectionChanged:
            m_SpriteComponent->m_Index = m_BlinkySpriteIndexOffset +
                    m_NrColumnsSprite * static_cast<int>(m_GridMovementComponent->GetDirection());
            break;
        case Events::GameOver:
        case Events::LevelWon:
        case Events::LevelLost:
        case Events::GhostDied:
        case Events::BeginLevel:
        case Events::RestartLevel:
        case Events::ScoreChanged:
        case Events::LivesChanged:
        case Events::InvincibilityChanged:
        case Events::Collision:
        case Events::NoEvent:
            break;
    }
}

void BlinkyComponent::Kill()
{
    std::cout << FUNCTION_NAME << '\n';
    m_bPendingKilled                                                  = true;
    m_Owner->GetComponent<HitboxComponent>()->m_bAreCollisionsEnabled = false;
}

void BlinkyComponent::UpdateToNewState(std::unique_ptr<States::EntityState> newState)
{
    if(!newState && !m_bPendingKilled)
    {
        return;
    }

    m_BlinkyState->OnExit();

    if(m_bPendingKilled)
    {
        m_bPendingKilled = false;
        newState         = std::make_unique<States::BlinkyEaten>(*m_Owner, m_SpawnPosition);
    }

    m_BlinkyState = std::move(newState);
    m_BlinkyState->OnEnter();
}
