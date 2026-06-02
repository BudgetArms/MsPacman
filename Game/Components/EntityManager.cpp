#include "EntityManager.hpp"

using namespace Game;


EntityManager::EntityManager(bae::Scene* scene, bae::GameObject* gameObject) :
    m_Scene{ scene },
    m_GameObject{ gameObject }
{
}


LevelGridComponent* EntityManager::GetLevelGridComponent() const
{
    if(!m_GameObject)
    {
        return nullptr;
    }

    
    return m_GameObject->GetComponent<LevelGridComponent>();
}

