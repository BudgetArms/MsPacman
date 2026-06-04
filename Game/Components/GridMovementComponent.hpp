#pragma once

#include "Components/Component.hpp"

#include "Base/DirectionEnum.hpp"


namespace Game
{
    class LevelGridComponent;

    class GridMovementComponent : public bae::Component
    {
    public:
        explicit GridMovementComponent(bae::GameObject& owner, LevelGridComponent& gridComponent);

        void Update() override;

        void Move(Direction direction);

        [[nodiscard]] bool CanMoveInDirection(Direction direction) const;

        float m_Speed{ 100.f };

    private:
        void LockOnGrid() const;

        LevelGridComponent* m_LevelGridComponent;

        Direction m_CurrentDirection{ Direction::Left };
    };
}
