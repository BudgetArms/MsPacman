#pragma once

#include "States/Entities/GhostStates.hpp"

#include "Base/GhostMovement.hpp"


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
        static std::vector<bae::GameObject*> GetPlayersGameObjects();

        GhostMovement m_Movement;
    };
}
