#pragma once

#include <vector>

#include "Components/Component.hpp"


namespace Game
{
    class HitboxComponent;

    class CollisionManagerComponent : public bae::Component
    {
    public:
        explicit CollisionManagerComponent(bae::GameObject& owner);

        void FixedUpdate() override;


        void RegisterHitbox(const HitboxComponent& hitbox);
        void UnRegisterHitbox(HitboxComponent* hitbox);

    private:
        static void SendHitboxesNotifications(HitboxComponent* hitboxA, HitboxComponent* hitboxB);


        std::vector<bae::GameObject*> m_HitboxObjects{};
    };
}
