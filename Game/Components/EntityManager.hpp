#pragma once

#include "Core/Scene.hpp"
#include "Singletons/Singleton.hpp"

#include "Components/LevelGridComponent.hpp"


namespace Game
{
    class EntityManager : public bae::Singleton<EntityManager>
    {
    public:
        EntityManager(bae::Scene* scene, bae::GameObject* gameObject);


        [[nodiscard]] LevelGridComponent* GetLevelGridComponent() const;

    private:
        ~EntityManager() override = default;


        bae::Scene* m_Scene;
        bae::GameObject* m_GameObject;
    };
}
