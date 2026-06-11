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

void GhostMovement::FixedUpdate()
{
    // todo: fix the testing
    if(m_bDirtyTargetPosition)
    {
    }
    if(m_PathToTarget.empty())
    {
        if(IsNearIntersection() && m_bDirtyTargetPosition)
        {
            std::cout << "YESSS: " << IsNearIntersection() << std::endl;
            m_TargetPosition = m_RequestedTargetPosition;
        }
    }
    else
    {
        if(IsCloseToNode(m_GhostObject->GetWorldLocation()) &&
            IsNearIntersection() &&
            m_bDirtyTargetPosition)
        {
            m_TargetPosition = m_RequestedTargetPosition;
            std::cout << "IsNearIntersection: " << IsNearIntersection() << std::endl;
        }
    }

    if(IsNearIntersection())
    {
        UpdatePath();
    }

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

    if(!IsNearIntersection())
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

    if(IsCloseToNode(currentPos))
    {
        m_PathToTarget.erase(m_PathToTarget.begin());
    }
}

bool GhostMovement::IsCloseToNode(const glm::vec2& position) const
{
    const auto* levelGrid = m_GridMovementComponent->GetLevelGridComponent();
    const auto gridPos    = levelGrid->GetClosestValidNodePosition(position);

    if(!levelGrid->IsInGrid(gridPos))
    {
        return false;
    }

    const glm::vec2 nodePosition = levelGrid->GetPosition(gridPos);

    return glm::distance(position, nodePosition) < m_MinDistanceToCell;
}

bool GhostMovement::IsNearIntersection() const
{
    if(!IsCloseToNode(m_GhostObject->GetWorldLocation()))
    {
        return false;
    }

    int nrMoveableDirections{};

    const auto levelGridComp = m_GridMovementComponent->GetLevelGridComponent();
    const auto gridPos       = levelGridComp->GetGridPosition(m_GhostObject->GetWorldLocation());

    constexpr int amountOfDirections{ 4 };
    for(int i{}; i < amountOfDirections; ++i)
    {
        if(levelGridComp->DoesConnectionExistInDirection(gridPos, static_cast<Direction>(i)))
        {
            ++nrMoveableDirections;
        }
    }

    return nrMoveableDirections > 2;
}

bool GhostMovement::IsNearTJunction() const
{
    if(!IsCloseToNode(m_GhostObject->GetWorldLocation()))
    {
        return false;
    }

    const Direction currentDirection = m_GridMovementComponent->GetDirection();
    int nrMoveableDirections{};

    const auto levelGridComp = m_GridMovementComponent->GetLevelGridComponent();
    const auto gridPos       = levelGridComp->GetGridPosition(m_GhostObject->GetWorldLocation());

    if(levelGridComp->DoesConnectionExistInDirection(gridPos, currentDirection))
    {
        return false;
    }

    constexpr int amountOfDirections{ 4 };
    for(int i{}; i < amountOfDirections; ++i)
    {
        if(levelGridComp->DoesConnectionExistInDirection(gridPos, static_cast<Direction>(i)))
        {
            ++nrMoveableDirections;
        }
    }

    return nrMoveableDirections == 3;
}
