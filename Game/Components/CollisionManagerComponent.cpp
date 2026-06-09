#include "CollisionManagerComponent.hpp"


#include "Components/HitboxComponent.hpp"


using namespace Game;


CollisionManagerComponent::CollisionManagerComponent(bae::GameObject& owner) :
    Component(owner)
{
}

void CollisionManagerComponent::FixedUpdate()
{
    if(m_Hitboxes.size() < 2)
    {
        return;
    }

    HitboxComponent* hitboxComponentA{};
    HitboxComponent* hitboxComponentB{};

    for(size_t i = 0; i < m_Hitboxes.size(); ++i)
    {
        hitboxComponentA = m_Hitboxes[i];
        for(size_t j = i + 1; j < m_Hitboxes.size(); ++j)
        {
            hitboxComponentB = m_Hitboxes[j];

            if(!hitboxComponentA || !hitboxComponentB)
            {
                continue;
            }

            const SDL_FRect hitboxA = hitboxComponentA->GetHitbox();
            const SDL_FRect hitboxB = hitboxComponentB->GetHitbox();
            if(SDL_HasRectIntersectionFloat(&hitboxA, &hitboxB))
            {
                SendHitboxesNotifications(hitboxComponentA, hitboxComponentB);
            }
        }
    }
}

void CollisionManagerComponent::RegisterHitbox(HitboxComponent& hitbox)
{
    const auto hitboxIt = std::ranges::find_if(m_Hitboxes, [&](const HitboxComponent* hitboxComponent)
    {
        return &hitbox == hitboxComponent;
    });

    if(hitboxIt != m_Hitboxes.end())
    {
        return;
    }

    m_Hitboxes.push_back(&hitbox);
}


void CollisionManagerComponent::SendHitboxesNotifications(HitboxComponent* hitboxA, HitboxComponent* hitboxB)
{
    if(!hitboxA || !hitboxB ||
        !hitboxA->GetGameObject() || !hitboxB->GetGameObject() ||
        hitboxA->GetGameObject()->IsMarkedForDeletion() || hitboxB->GetGameObject()->IsMarkedForDeletion()
    )
    {
        return;
    }

    hitboxA->SendCollisionEventToObservers(*hitboxB);
    hitboxB->SendCollisionEventToObservers(*hitboxA);
}
