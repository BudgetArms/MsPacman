#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


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

        [[nodiscard]] bool IsCloseToNode(const glm::vec2& position) const;
        [[nodiscard]] bool IsNearIntersection() const;
        [[nodiscard]] bool IsNearTJunction() const;
        [[nodiscard]] bool CanChangeDirection() const;


        bae::GameObject* m_GhostObject{};
        GridMovementComponent* m_GridMovementComponent{};

        glm::vec2 m_TargetPosition{};
        glm::vec2 m_RequestedTargetPosition{};

        std::vector<glm::vec2> m_PathToTarget{};

        const float m_MinDistanceToCell{ 5.f };
        const float m_RecalculatePathCooldownTime{ 0.5f };
        float m_ElapsedTime{};

        bool m_bDirtyTargetPosition{};
    };
}
