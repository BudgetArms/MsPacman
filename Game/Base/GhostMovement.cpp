#include "GhostMovement.hpp"

#include "Singletons/GameTime.hpp"

#include "Components/GridMovementComponent.hpp"
#include "Components/LevelGridComponent.hpp"


using namespace Game;


GhostMovement::GhostMovement(bae::GameObject& ghostObject, GridMovementComponent& gridMovementComponent, float speed) :
    m_Speed{ speed },
    m_GhostObject{ &ghostObject },
    m_GridMovementComponent{ &gridMovementComponent }
{
}

GhostMovement::~GhostMovement() = default;

void GhostMovement::Update()
{
    UpdatePath();
void GhostMovement::FixedUpdate()
{
    SetDirectionFromPath();
}

void GhostMovement::SetTargetPosition(const glm::vec2& targetPosition)
{
    if(m_TargetPosition == targetPosition)
    {
        m_bDirtyTargetPosition = false;
        return;
    }

    m_bDirtyTargetPosition    = true;
    m_RequestedTargetPosition = targetPosition;
}


void GhostMovement::UpdatePath()
{
    m_ElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_ElapsedTime < m_RecalculatePathCooldownTime)
    {
        return;
    }

    m_ElapsedTime = 0.0f;

    const glm::vec2 currentPos = m_GhostObject->GetWorldLocation();
    const auto levelGridComp   = m_GridMovementComponent->GetLevelGridComponent();
    m_PathToTarget             = levelGridComp->GetShortestPath(currentPos, m_TargetPosition);
    if(m_PathToTarget.empty())
    {
        return;
    }


    if(m_PathToTarget.empty())
    {
        return;
    }

    if(glm::distance(currentPos, m_PathToTarget.front()) < m_MinDistanceToCell)
    {
        m_PathToTarget.erase(m_PathToTarget.begin());
    }
}

void GhostMovement::SetDirectionFromPath()
{
    if(m_PathToTarget.empty())
    {
        return;
    }

    const glm::vec2 currentPos   = m_GhostObject->GetWorldLocation();
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

    m_GridMovementComponent->SetDirection(direction);

    if(glm::distance(currentPos, pathPosition) < m_MinDistanceToCell)
    {
        m_PathToTarget.erase(m_PathToTarget.begin());
    }
}

