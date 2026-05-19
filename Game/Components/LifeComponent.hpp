#pragma once

#include "Components/Component.hpp"
#include "Core/Subject.hpp"


namespace Game
{
    class LifeComponent : public bae::Component, public bae::Subject
    {
    public:
        explicit LifeComponent(bae::GameObject& owner, int maxLives);

        void AddLife();
        void RemoveLife();

        void RemoveAllLives();

        [[nodiscard]] int GetLives() const;
        void SetLives(int lives);

        [[nodiscard]] int GetMaxLives() const;
        void SetMaxLives(int maxLives);

        [[nodiscard]] bool IsInvincible() const;
        void SetInvincibility(bool isInvincible);

        [[nodiscard]] bool IsAlive() const;

    private:
        int m_Lives;
        int m_MaxLives;

        bool m_bIsInvincible{ false };
        bool m_bIsAlive{ true };
    };
}
