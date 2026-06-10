#pragma once

#include "Components/Component.hpp"

#include "States/Entities/GhostStates.hpp"


namespace Game
{
    class BlinkyComponent : public bae::Component
    {
    public:
        explicit BlinkyComponent(bae::GameObject& owner);
        ~BlinkyComponent() override;

        void Update() override;

        [[nodiscard]] States::EntityState* GetState() const;

        void Kill();

    private:
        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);


        std::unique_ptr<States::EntityState> m_BlinkyState;

        bool m_bPendingKilled{};
    };
}
