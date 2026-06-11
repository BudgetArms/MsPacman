#pragma once

#include "Components/Component.hpp"
#include "Core/Subject.hpp"

#include "Base/Events.hpp"


namespace Game
{
    class LifeComponent : public bae::Component, public bae::Subject
    {
    public:
        explicit LifeComponent(bae::GameObject& owner, int maxLives, float invincibilityDuration);

        void Update() override;


        // Respects invincibility
        void AddLife();

        // Respects invincibility
        void RemoveLife();

        // Doesn't care about invincibility
        void RemoveAllLives();

        [[nodiscard]] int GetLives() const;
        void SetLives(int lives);

        [[nodiscard]] int GetMaxLives() const;
        void SetMaxLives(int maxLives);

        [[nodiscard]] bool IsInvincible() const;
        void SetInvincibility(bool isInvincible);

        [[nodiscard]] bool IsAlive() const;

    private:
        void SendEventToObservers(Events event);

        int m_Lives;
        int m_MaxLives;

        const float m_InvincibilityDuration;
        float m_ElapsedInvincibilityTime{};


        bool m_bIsInvincible{ false };
        bool m_bIsAlive{ true };
    };
}
