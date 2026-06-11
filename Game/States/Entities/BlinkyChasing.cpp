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
    GhostStates(gameObject)
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
    // UpdateTargetPosition();
    m_TargetPosition = { 305, 102 };
    UpdatePath();
    SetDirectionFromPath();

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

void States::BlinkyChasing::UpdatePath()
{
    m_ElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_ElapsedTime < m_RecalculatePathCooldownTime)
    {
        return;
    }

    m_ElapsedTime = 0.0f;

    const glm::vec2 currentPos = m_GameObject->GetWorldLocation();
    const auto levelGridComp   = m_GridMovementComponent->GetLevelGridComponent();
    m_PathToTarget             = levelGridComp->GetShortestPath(currentPos, m_TargetPosition);
    if(m_PathToTarget.empty())
    {
        return;
    }


    if(m_PathToTarget.empty())
    {
        return;
    }

    if(glm::distance(currentPos, m_PathToTarget.front()) < m_MinDistanceToCell)
    {
        m_PathToTarget.erase(m_PathToTarget.begin());
    }
}

void States::BlinkyChasing::SetDirectionFromPath()
{
    if(m_PathToTarget.empty())
    {
        return;
    }

    const glm::vec2 currentPos   = m_GameObject->GetWorldLocation();
    const glm::vec2 pathPosition = m_PathToTarget.front();

    const glm::vec2 directionToPathPos = pathPosition - currentPos;

    Direction direction{};
    if(std::abs(directionToPathPos.x) > std::abs(directionToPathPos.y))
    {
        // Left or Right
        if(directionToPathPos.x < 0)
        {
            direction = Direction::Left;
        }
        else
        {
            direction = Direction::Right;
        }
    }
    else
    {
        // Down or Up
        if(directionToPathPos.y < 0)
        {
            direction = Direction::Up;
        }
        else
        {
            direction = Direction::Down;
        }
    }

    m_GridMovementComponent->SetDirection(direction);

    if(glm::distance(currentPos, pathPosition) < m_MinDistanceToCell)
    {
        m_PathToTarget.erase(m_PathToTarget.begin());
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
