#pragma once

#include "../States/Entities/EntityState.hpp"
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
        ~MsPacmanComponent() override;

        void Update() override;

    private:
        // std::unique_ptr<States::MsPacmanState> m_MsPacmanState;
        std::unique_ptr<States::EntityState> m_MsPacmanState;
    };
}
