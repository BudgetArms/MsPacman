#pragma once

#include "Core/EventListener.hpp"
#include "Sounds/SoundStructs.hpp"

#include "States/Entities/EntityState.hpp"
#include "States/Entities/MsPacmanStates.hpp"


namespace Game::States
{
    class MsPacmanMoving final : public MsPacmanState
    {
    public:
        explicit MsPacmanMoving(bae::GameObject& gameObject)
            : MsPacmanState(gameObject) {}

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;
        void HandleInput() override;
    };
}

