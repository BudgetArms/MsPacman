#include "GhostMovement.hpp"

#include "Core/HelperFunctions.hpp"

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
    if(IsOnSingleRoadNode() && !IsOnSingleRoadNode())
    {
        std::cout << "SingleRoad Node" << '\n';
        std::set<Direction> possibleMoveDirections = GetPossibleMoveDirections();
        possibleMoveDirections.erase(GetOppositeDirection());

        // Since single road, it will only return two directions
        // If I remove the back direction, it will go to the other possible direction
        SetDirection(*possibleMoveDirections.begin());
        return;
    }


    UpdatePath();
    SetDirectionFromPath();
}

void GhostMovement::SetTargetPosition(const glm::vec2& targetPosition)
{
    m_TargetPosition = targetPosition;
}


void GhostMovement::UpdatePath()
{
    const glm::vec2 currentPos = m_GhostObject->GetWorldLocation();
    const auto levelGridComp   = m_GridMovementComponent->GetLevelGridComponent();

    m_PathToTarget = levelGridComp->GetShortestPath(currentPos, m_TargetPosition);
    if(m_PathToTarget.empty())
    {
        return;
    }

    if(IsCloseToNode())
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

    if(!IsCloseToNode())
    {
        return;
    }


    const glm::vec2 pathPosition = m_PathToTarget.front();

    const glm::vec2 currentPos         = m_GhostObject->GetWorldLocation();
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

    SetDirection(direction);
}

Direction GhostMovement::GetCurrentDirection() const
{
    return m_GridMovementComponent->GetDirection();
}

void GhostMovement::SetDirection(Direction direction) const
{
    m_GridMovementComponent->SetDirection(direction);
}

Direction GhostMovement::GetOppositeDirection() const
{
    switch(GetCurrentDirection())
    {
        case Direction::Right:
            return Direction::Left;
        case Direction::Left:
            return Direction::Right;
        case Direction::Up:
            return Direction::Down;
        case Direction::Down:
            return Direction::Up;
    }

    throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! You shouldn't be able to get here!"));
}

std::set<Direction> GhostMovement::GetPossibleMoveDirections() const
{
    if(!IsCloseToNode())
    {
        return std::set<Direction>{};
    }

    const auto levelGridComp = m_GridMovementComponent->GetLevelGridComponent();
    const auto gridPos       = levelGridComp->GetGridPosition(m_GhostObject->GetWorldLocation());

    std::set<Direction> possibleDirections{};

    constexpr int amountOfDirections{ 4 };
    for(int i{}; i < amountOfDirections; ++i)
    {
        if(levelGridComp->DoesConnectionExistInDirection(gridPos, static_cast<Direction>(i)))
        {
            possibleDirections.insert(static_cast<Direction>(i));
        }
    }

    return possibleDirections;
}

bool GhostMovement::IsCloseToNode() const
{
    const glm::vec2 currentPos = m_GhostObject->GetWorldLocation();
    return IsCloseToNodeAtPosition(currentPos);
}

bool GhostMovement::IsCloseToNodeAtPosition(const glm::vec2& position) const
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

bool GhostMovement::IsOnSingleRoadNode() const
{
    return GetPossibleMoveDirections().size() == 2;
}

bool GhostMovement::IsOnStraightRoadNode() const
{
    if(!IsOnSingleRoadNode())
    {
        return false;
    }

    auto possibleDirections = GetPossibleMoveDirections();
    possibleDirections.erase(GetCurrentDirection());
    possibleDirections.erase(GetOppositeDirection());

    return possibleDirections.empty();
}

bool GhostMovement::IsOnDeadEndNode() const
{
    return GetPossibleMoveDirections().size() == 1;
}

bool GhostMovement::IsOnIntersectionNode() const
{
    return GetPossibleMoveDirections().size() > 2;
}
