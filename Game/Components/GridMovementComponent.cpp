#include "GridMovementComponent.hpp"

#include <glm/glm.hpp>

#include "Components/LevelGridComponent.hpp"
#include "Singletons/GameTime.hpp"


using namespace Game;


GridMovementComponent::GridMovementComponent(bae::GameObject& owner, LevelGridComponent& gridComponent) :
    Component(owner),
    m_LevelGridComponent{ &gridComponent }
{
}

void GridMovementComponent::Update()
{
    if(!CanMoveInDirection(m_CurrentDirection))
    {
        return;
    }

    const float deltaSpeed = m_Speed * bae::GameTime::GetInstance().GetDeltaTime();

    switch(m_CurrentDirection)
    {
        case Direction::Left:
            m_Owner->AddLocation({ -deltaSpeed, 0 });
            break;
        case Direction::Right:
            m_Owner->AddLocation({ deltaSpeed, 0 });
            break;
        case Direction::Up:
            m_Owner->AddLocation({ 0, -deltaSpeed });
            break;
        case Direction::Down:
            m_Owner->AddLocation({ 0, deltaSpeed });
            break;
    }

    LockOnGrid();
}

void GridMovementComponent::Move(const Direction direction)
{
    if(!CanMoveInDirection(direction))
    {
        return;
    }

    m_CurrentDirection = direction;
}

bool GridMovementComponent::CanMoveInDirection(const Direction direction) const
{
    const glm::vec2& position                    = m_Owner->GetWorldLocation();
    const bae::Graphs::GridPosition gridPosition = m_LevelGridComponent->GetGridPosition(position);

    return m_LevelGridComponent->DoesConnectionExistInDirection(gridPosition, direction);
}

void GridMovementComponent::LockOnGrid() const
{
    const glm::vec2& position                    = m_Owner->GetWorldLocation();
    const bae::Graphs::GridPosition gridPosition = m_LevelGridComponent->GetGridPosition(position);
    glm::vec2 positionOnGrid                     = m_LevelGridComponent->GetPosition(gridPosition);

    switch(m_CurrentDirection)
    {
        case Direction::Left:
        case Direction::Right:
            positionOnGrid.x = position.x;
            break;
        case Direction::Up:
        case Direction::Down:
            positionOnGrid.y = position.y;
            break;
    }

    m_Owner->SetWorldLocation(positionOnGrid);
}
