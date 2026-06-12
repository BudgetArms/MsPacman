#pragma once

#include "States/Entities/GhostStates.hpp"

#include "Base/GhostMovement.hpp"


namespace Game::States
{
    class BlinkyEaten : public GhostStates
    {
    public:
        explicit BlinkyEaten(bae::GameObject& gameObject, const glm::vec2& spawnPosition);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

    private:
        GhostMovement m_Movement;

        const float m_TimeDeathCooldownTime{ 6.f };
        float m_TimeDeathElapsed{};
    };
}
