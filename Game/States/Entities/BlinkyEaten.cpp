#include "BlinkyEaten.hpp"

#include "Core/HelperFunctions.hpp"
#include "Singletons/GameTime.hpp"

#include "Components/HitboxComponent.hpp"
#include "States/Entities/BlinkyChasing.hpp"


using namespace Game;


States::BlinkyEaten::BlinkyEaten(bae::GameObject& gameObject, const glm::vec2& spawnPosition) :
    GhostStates(gameObject),
    m_Movement{ gameObject, *m_GridMovementComponent, 50.f }
{
    m_TargetPosition = spawnPosition;
}

void States::BlinkyEaten::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';
    m_GameObject->GetComponent<HitboxComponent>()->m_bAreCollisionsEnabled = false;

    m_Movement.SetTargetPosition(m_TargetPosition);
}

void States::BlinkyEaten::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
    m_GameObject->GetComponent<HitboxComponent>()->m_bAreCollisionsEnabled = true;
}

std::unique_ptr<States::EntityState> States::BlinkyEaten::Update()
{
    m_TimeDeathElapsed += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_TimeDeathElapsed >= m_TimeDeathCooldownTime)
    {
        return std::make_unique<BlinkyChasing>(*m_GameObject);
    }

    m_Movement.FixedUpdate();
    return nullptr;
}

