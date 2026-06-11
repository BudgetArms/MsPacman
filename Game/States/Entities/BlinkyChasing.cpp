#include "BlinkyChasing.hpp"

#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"
#include "Singletons/GameTime.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Components/LevelGridComponent.hpp"
#include "Components/MsPacmanComponent.hpp"


using namespace Game;


States::BlinkyChasing::BlinkyChasing(bae::GameObject& gameObject) :
    GhostStates(gameObject),
    m_Movement{ gameObject, *m_GridMovementComponent, 50.f }
{
}

void States::BlinkyChasing::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';
}

void States::BlinkyChasing::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

std::unique_ptr<States::EntityState> States::BlinkyChasing::Update()
{
    UpdateTargetPosition();

    m_Movement.SetTargetPosition(m_TargetPosition);
    m_Movement.FixedUpdate();

    return nullptr;
}

void States::BlinkyChasing::UpdateTargetPosition()
{
    const auto players = GetPlayersGameObjects();
    if(players.empty())
    {
        return;
    }

    const glm::vec2 position = m_GameObject->GetWorldLocation();
    float closestDistance    = FLT_MAX;

    for(const auto& object : players)
    {
        const glm::vec2 playerPosition = object->GetWorldLocation();
        const float newDistance        = glm::distance(position, playerPosition);

        if(newDistance <= closestDistance)
        {
            closestDistance  = newDistance;
            m_TargetPosition = playerPosition;
        }
    }
}

std::vector<bae::GameObject*> States::BlinkyChasing::GetPlayersGameObjects()
{
    bae::Scene* levelScene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    std::vector<bae::GameObject*> players{};

    for(const auto& object : levelScene->GetObjects())
    {
        if(object->HasComponent<MsPacmanComponent>())
        {
            players.push_back(object.get());
        }
    }

    return players;
}
