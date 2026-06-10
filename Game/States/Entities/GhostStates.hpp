#pragma once

#include "Components/GridMovementComponent.hpp"
#include "Core/GameObject.hpp"

#include "States/Entities/EntityState.hpp"


namespace Game::States
{
    class GhostStates : public EntityState
    {
    public:
        explicit GhostStates(bae::GameObject& gameObject);

        void OnResume() override;
        void OnSuspend() override;

        virtual void HandleInput() {};

        [[nodiscard]] bool IsPaused() const;

    private:
        bool m_bIsPaused{};

    protected:
        bae::GameObject* m_GameObject;
        GridMovementComponent* m_GridMovementComponent{};

        glm::vec2 m_TargetPosition{};
    };
}
