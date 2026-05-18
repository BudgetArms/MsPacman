#pragma once

#include "Core/GameObject.hpp"

#include "EntityState.hpp"


namespace Game::States
{
    class MsPacmanState : public EntityState
    {
    public:
        explicit MsPacmanState(bae::GameObject& gameObject);

        void OnResume() final;
        void OnSuspend() final;

        virtual void HandleInput() = 0;

    private:
        bool m_bIsInputPaused{ false };

    protected:
        [[nodiscard]] bool IsInputPaused() const;

        bae::GameObject* m_GameObject;
    };
}

