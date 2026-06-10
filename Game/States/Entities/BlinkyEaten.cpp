#include "BlinkyEaten.hpp"

#include "Core/HelperFunctions.hpp"
#include "Singletons/GameTime.hpp"

#include "Components/LevelGridComponent.hpp"
#include "States/Entities/BlinkyChasing.hpp"


using namespace Game;


States::BlinkyEaten::BlinkyEaten(bae::GameObject& gameObject) :
    GhostStates(gameObject)
{
}

void States::BlinkyEaten::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';
}

void States::BlinkyEaten::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

std::unique_ptr<States::EntityState> States::BlinkyEaten::Update()
{
    UpdatePath();
    SetDirectionFromPath();

    m_TimeDeathElapsed += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_TimeDeathElapsed >= m_TimeDeathCooldownTime)
    {
        return std::make_unique<BlinkyChasing>(*m_GameObject);
    }


    return nullptr;
}


void States::BlinkyEaten::UpdatePath()
{
    m_PathUpdatedElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_PathUpdatedElapsedTime < m_RecalculatePathCooldownTime)
    {
        return;
    }

    m_PathUpdatedElapsedTime = 0.0f;

    const glm::vec2 currentPos = m_GameObject->GetWorldLocation();
    const auto levelGridComp   = m_GridMovementComponent->GetLevelGridComponent();
    m_PathToTarget             = levelGridComp->GetShortestPath(currentPos, m_TargetPosition);

    if(m_PathToTarget.empty())
    {
        return;
    }

    if(glm::distance(currentPos, m_PathToTarget.front()) < m_MinDistanceToCell)
    {
        m_PathToTarget.erase(m_PathToTarget.begin());
    }
}

void States::BlinkyEaten::SetDirectionFromPath()
{
    if(m_PathToTarget.empty())
    {
        return;
    }

    const glm::vec2 currentPos   = m_GameObject->GetWorldLocation();
    const glm::vec2 pathPosition = m_PathToTarget.front();

    const glm::vec2 directionToPathPos = pathPosition - currentPos;

    Direction direction{};
    if(std::abs(directionToPathPos.x) > std::abs(directionToPathPos.y))
    {
        // Left or Right
        if(directionToPathPos.x < 0)
        {
            direction = Direction::Left;
        }
        else
        {
            direction = Direction::Right;
        }
    }
    else
    {
        // Down or Up
        if(directionToPathPos.y < 0)
        {
            direction = Direction::Up;
        }
        else
        {
            direction = Direction::Down;
        }
    }

    m_GridMovementComponent->Move(direction);

    if(glm::distance(currentPos, pathPosition) < m_MinDistanceToCell)
    {
        m_PathToTarget.erase(m_PathToTarget.begin());
    }
}
