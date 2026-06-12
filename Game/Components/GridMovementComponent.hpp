#pragma once

#include "Components/Component.hpp"
#include "Core/Subject.hpp"

#include "Base/DirectionEnum.hpp"


namespace Game
{
    class LevelGridComponent;

    class GridMovementComponent : public bae::Component, public bae::Subject
    {
    public:
        explicit GridMovementComponent(bae::GameObject& owner, LevelGridComponent& gridComponent);

        void FixedUpdate() override;


        [[nodiscard]] Direction GetDirection() const;
        void SetDirection(Direction direction);

        [[nodiscard]] bool CanMoveInDirection(Direction direction) const;

        [[nodiscard]] LevelGridComponent* GetLevelGridComponent() const;

        float m_Speed{ 100.f };

    private:
        void ApplySpeed() const;
        void LockOnGrid() const;

        [[nodiscard]] bool IsAtNodeCenter();

        LevelGridComponent* m_LevelGridComponent;

        Direction m_CurrentDirection{ Direction::Left };
        Direction m_RequestedDirection{ Direction::Left };


        const float m_ToleranceNodeCenter{ 5.f };
    };
}
