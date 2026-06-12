#include "BlinkyFrightened.hpp"

#include "BlinkyFrightened.hpp"

#include "Base/Events.hpp"
#include "Core/HelperFunctions.hpp"
#include "Singletons/GameTime.hpp"

#include "Components/HitboxComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "States/Entities/BlinkyChasing.hpp"


using namespace Game;


States::BlinkyFrightened::BlinkyFrightened(bae::GameObject& gameObject, const glm::vec2& frightenedPosition) :
    GhostStates(gameObject),
    m_Movement{ gameObject, *m_GridMovementComponent, 50.f }
{
    m_TargetPosition = frightenedPosition;
}

void States::BlinkyFrightened::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';
    m_Movement.SetTargetPosition(m_TargetPosition);

    const auto spriteComponent = m_GameObject->GetComponent<bae::SpriteComponent>();

    spriteComponent->SetTexture(m_SpritePath.data());
    spriteComponent->SetSrcRect(m_SrcRect);
    spriteComponent->SetColumns(m_NrColumnsSprite);
    spriteComponent->SetSprites(m_NrSprites);

    spriteComponent->m_Index = 1;
}

void States::BlinkyFrightened::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

std::unique_ptr<States::EntityState> States::BlinkyFrightened::Update()
{
    m_Movement.FixedUpdate();

    if(!m_bIsFrightened)
    {
        return std::make_unique<BlinkyChasing>(*m_GameObject);
    }

    return nullptr;
}

void States::BlinkyFrightened::HandleEvent(const unsigned eventHash)
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
            m_bIsFrightened = false;
        }
        break;
        case Events::Collision:
        case Events::NoEvent:
            break;
    }
}

