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

void GridMovementComponent::FixedUpdate()
{
    if(IsAtNodeCenter())
    {
        // std::cout << "AtNodeCenter\n";

        // this will lock even if impossible to go in requested pos
        // if(m_CurrentDirection != m_RequestedDirection)
        // this will ignore the mirror and passes past the level??
        // this will LockOnGrid when going to change directions
        if(m_CurrentDirection != m_RequestedDirection && CanMoveInDirection(m_RequestedDirection))
        {
            std::cout << "LockOnGrid" << '\n';
            LockOnGrid();
            m_CurrentDirection = m_RequestedDirection;
        }

        if(!CanMoveInDirection(m_CurrentDirection))
        {
            std::cout << "impossible to move in this direction" << '\n';
            return;
        }
    }
    std::cout << "\n";

    ApplySpeed();
}

void GridMovementComponent::Render() const
{
    bae::Utils::DrawCircle(m_NodePosTest, 10, bae::Utils::Color::Green);
}

void GridMovementComponent::SetDirection(const Direction direction)
{
    m_RequestedDirection = direction;
}

bool GridMovementComponent::CanMoveInDirection(const Direction direction) const
{
    const glm::vec2& position = m_Owner->GetWorldLocation();

    const bae::Graphs::GridPosition gridPosition = m_LevelGridComponent->GetClosestValidNodePosition(position);

    if(gridPosition.Column < 0 || gridPosition.Row < 0 ||
        gridPosition.Row > m_LevelGridComponent->GetRows() || gridPosition.Column > m_LevelGridComponent->GetColumns())
    {
        return false;
    }

    return m_LevelGridComponent->DoesConnectionExistInDirection(gridPosition, direction);
}

LevelGridComponent* GridMovementComponent::GetLevelGridComponent() const
{
    return m_LevelGridComponent;
}

void GridMovementComponent::ApplySpeed() const
{
    const float fixedSpeed = m_Speed * bae::GameTime::GetInstance().GetFixedTimeStep();

    switch(m_CurrentDirection)
    {
        case Direction::Left:
            m_Owner->AddLocation({ -fixedSpeed, 0 });
            break;

        case Direction::Right:
            m_Owner->AddLocation({ fixedSpeed, 0 });
            break;

        case Direction::Up:
            m_Owner->AddLocation({ 0, -fixedSpeed });
            break;

        case Direction::Down:
            m_Owner->AddLocation({ 0, fixedSpeed });
            break;
    }
}

void GridMovementComponent::LockOnGrid() const
{
    const glm::vec2& position = m_Owner->GetWorldLocation();

    const auto gridPosition = m_LevelGridComponent->GetClosestValidNodePosition(position);

    if(gridPosition.Column < 0 || gridPosition.Row < 0 ||
        gridPosition.Row > m_LevelGridComponent->GetRows() || gridPosition.Column > m_LevelGridComponent->GetColumns())
    {
        return;
    }

    m_Owner->SetWorldLocation(m_LevelGridComponent->GetPosition(gridPosition));
}

bool GridMovementComponent::IsAtNodeCenter()
{
    const glm::vec2& position = m_Owner->GetWorldLocation();

    const auto gridPosition = m_LevelGridComponent->GetClosestValidNodePosition(position);

    if(gridPosition.Column < 0 || gridPosition.Row < 0 ||
        gridPosition.Row > m_LevelGridComponent->GetRows() || gridPosition.Column > m_LevelGridComponent->GetColumns())
    {
        return false;
    }

    m_NodePosTest = position;

    const glm::vec2 nodePosition = m_LevelGridComponent->GetPosition(gridPosition);

    return glm::distance(position, nodePosition) <= m_ToleranceNodeCenter;
}
