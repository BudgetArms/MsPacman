#pragma once

#include <set>
#include <vector>

#include <glm/vec2.hpp>

#include "Base/DirectionEnum.hpp"


namespace bae
{
    class GameObject;
}

namespace Game
{
    class GridMovementComponent;

    class GhostMovement
    {
    public:
        explicit GhostMovement(bae::GameObject& ghostObject, GridMovementComponent& gridMovementComponent, float speed);
        virtual ~GhostMovement();

        void FixedUpdate();

        void SetTargetPosition(const glm::vec2& targetPosition);


        float m_Speed;

    private:
        void UpdatePath();
        void SetDirectionFromPath();


        [[nodiscard]] Direction GetCurrentDirection() const;
        void SetDirection(Direction direction) const;

        [[nodiscard]] Direction GetOppositeDirection() const;

        std::set<Direction> GetPossibleMoveDirections() const;

        [[nodiscard]] bool IsCloseToNode() const;
        [[nodiscard]] bool IsCloseToNodeAtPosition(const glm::vec2& position) const;

        [[nodiscard]] bool IsOnSingleRoadNode() const;
        [[nodiscard]] bool IsOnStraightRoadNode() const;
        [[nodiscard]] bool IsOnDeadEndNode() const;
        [[nodiscard]] bool IsOnIntersectionNode() const;


        bae::GameObject* m_GhostObject{};
        GridMovementComponent* m_GridMovementComponent{};

        glm::vec2 m_TargetPosition{};
        std::vector<glm::vec2> m_PathToTarget{};

        const float m_MinDistanceToCell{ 5.f };
    };
}
