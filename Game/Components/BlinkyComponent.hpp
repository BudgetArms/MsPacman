#pragma once


#include "Components/Component.hpp"

#include "Base/GhostKiller.hpp"
#include "States/Entities/GhostStates.hpp"


namespace bae
{
    class SpriteComponent;
}

namespace Game
{
    class LevelGridComponent;
    class GridMovementComponent;

    class BlinkyComponent : public bae::Component, public bae::Observer, public GhostKiller
    {
    public:
        explicit BlinkyComponent(bae::GameObject& owner, LevelGridComponent* levelGridComp);
        ~BlinkyComponent() override;

        void Update() override;

        [[nodiscard]] States::EntityState* GetState() const;


        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

        void Kill() override;

    private:
        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);

    private:
        std::unique_ptr<States::EntityState> m_BlinkyState;

        bae::SpriteComponent* m_SpriteComponent{};
        GridMovementComponent* m_GridMovementComponent{};

        static constexpr int m_SpriteIndexOffset{ 1 };
        static constexpr int m_NrColumnsSprite{ 2 };

        bool m_bPendingKilled{};
    };
}
