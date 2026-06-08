#pragma once

#include "Components/Component.hpp"
#include "Core/Observer.hpp"

#include "States/Entities/EntityState.hpp"


namespace Game::States
{
    class MsPacmanState;
}

namespace Game
{
    class MsPacmanComponent final : public bae::Component, public bae::Observer

    {
    public:
        explicit MsPacmanComponent(bae::GameObject& owner);
        ~MsPacmanComponent() override = default;

        void Update() override;

        [[nodiscard]] States::EntityState* GetState() const;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any&) override;

    private:
        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);

        std::unique_ptr<States::EntityState> m_MsPacmanState;
    };
}
