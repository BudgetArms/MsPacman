#pragma once

#include <set>

#include "Components/Component.hpp"
#include "Core/Observer.hpp"

#include "States/Entities/EntityState.hpp"


namespace bae
{
    class SpriteComponent;
}

namespace Game::States
{
    class MsPacmanState;
}

namespace Game
{
    class LevelGridComponent;
    class GridMovementComponent;
    class GhostKiller;
    class ItemComponent;

    class MsPacmanComponent final : public bae::Component, public bae::Observer
    {
    public:
        explicit MsPacmanComponent(bae::GameObject& owner, LevelGridComponent* levelGridComp);
        ~MsPacmanComponent() override = default;

        void Update() override;

        [[nodiscard]] States::EntityState* GetState() const;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any&) override;

    private:
        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);

        void HandleCollision(const std::any& eventData) const;
        void HandleItemCollision(ItemComponent& itemComponent) const;
        void HandleEnemyCollision(bae::GameObject* gameObject) const;

        [[nodiscard]] static bool IsEnemy(bae::GameObject* gameObject);
        [[nodiscard]] static GhostKiller* GetEnemyClass(bae::GameObject* gameObject);

        [[nodiscard]] static std::set<bae::GameObject*> GetEnemies();

        std::unique_ptr<States::EntityState> m_MsPacmanState{};

        bae::SpriteComponent* m_SpriteComponent{};
        GridMovementComponent* m_GridMovementComponent{};

        static constexpr int m_SpriteIndexOffset{ 1 };
        static constexpr int m_NrColumnsSprite{ 3 };
    };
}
