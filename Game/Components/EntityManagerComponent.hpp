#pragma once

#include "Core/Scene.hpp"

#include "Components/LevelGridComponent.hpp"
#include "Core/EventListener.hpp"


namespace Game
{
    class EntityManagerComponent : public bae::Component, public bae::EventListener
    {
    public:
        explicit EntityManagerComponent(bae::GameObject& owner);
        ~EntityManagerComponent() override = default;

        void AddPlayerToScene(std::shared_ptr<bae::GameObject> gameObject);
        void AddToScene(std::shared_ptr<bae::GameObject> gameObject);

    private:
        void HandleEvent(unsigned eventHash) override;
    };
}
