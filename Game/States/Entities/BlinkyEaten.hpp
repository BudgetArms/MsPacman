#pragma once
#include "GhostStates.hpp"


namespace Game::States
{
    class BlinkyEaten : public GhostStates
    {
    public:
        explicit BlinkyEaten(bae::GameObject& gameObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

    private:
        void UpdatePath();

        void SetDirectionFromPath();

        std::vector<glm::vec2> m_PathToTarget{};

        const float m_MinDistanceToCell{ 1.f };

        const float m_TimeDeathCooldownTime{ 6.f };
        float m_TimeDeathElapsed{};

        const float m_RecalculatePathCooldownTime{ 0.5f };
        float m_PathUpdatedElapsedTime{};
    };
}
