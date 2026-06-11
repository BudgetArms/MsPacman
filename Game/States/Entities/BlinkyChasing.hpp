#pragma once
#include "GhostStates.hpp"
#include "MsPacmanStates.hpp"


namespace Game::States
{
    class BlinkyChasing : public GhostStates
    {
    public:
        explicit BlinkyChasing(bae::GameObject& gameObject);

        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<EntityState> Update() override;

    private:
        void UpdateTargetPosition();
        void UpdatePath();

        void SetDirectionFromPath();

        static std::vector<bae::GameObject*> GetPlayersGameObjects();


        std::vector<glm::vec2> m_PathToTarget{};

        const float m_MinDistanceToCell{ 5.f };
        const float m_RecalculatePathCooldownTime{ 0.5f };
        float m_ElapsedTime{};
    };
}
