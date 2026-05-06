#pragma once

#include "Components/Component.h"


namespace Game::States
{
    class MsPacmanState;
}

namespace Game
{
    class MsPacmanComponent final : public bae::Component
    {
    public:
        explicit MsPacmanComponent(bae::GameObject& owner);
        ~MsPacmanComponent();

        void Update() override;

        void SetState(std::unique_ptr<Game::States::MsPacmanState> state);

    private:
        std::unique_ptr<Game::States::MsPacmanState> m_MsPacmanState;
    };
}
