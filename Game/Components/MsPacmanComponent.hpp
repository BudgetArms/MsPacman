#pragma once

#include "Components/Component.hpp"

#include "States/Entities/EntityState.hpp"


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
        ~MsPacmanComponent() override = default;

        void Update() override;

        [[nodiscard]] States::EntityState* GetState() const;

    private:
        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);


        // std::unique_ptr<States::MsPacmanState> m_MsPacmanState;
        std::unique_ptr<States::EntityState> m_MsPacmanState;
    };
}
