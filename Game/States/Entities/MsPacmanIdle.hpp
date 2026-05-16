#pragma once

#include "Sounds/SoundStructs.h"

#include "EntityState.hpp"
#include "MsPacmanStates.hpp"

namespace Game::States
{
    class MsPacmanIdle final : public MsPacmanState
    {
    public:
        explicit MsPacmanIdle(bae::GameObject& gameObject)
            : MsPacmanState(gameObject) {}

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;
        void HandleInput() override;

    private:
        bae::ActiveSoundID m_PlayingActiveSoundId{ -1 };

        // TODO: remove this code afterwards
        float m_AccumulatedTime{};
        static constexpr float m_TimeSwitch{ 3.f };
    };
}

