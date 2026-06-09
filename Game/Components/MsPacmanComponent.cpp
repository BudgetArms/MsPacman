#include "MsPacmanComponent.hpp"

#include "LevelManagerComponent.hpp"
#include "Base/CommonManagerVariables.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Core/GameObject.hpp"
#include "Managers/ResourceManager.hpp"

#include "Base/Events.hpp"
#include "Base/Level.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/ScoreComponent.hpp"
#include "Core/Scene.hpp"
#include "States/Entities/MsPacmanDying.hpp"
#include "States/Entities/MsPacmanIdle.hpp"


using namespace Game;


MsPacmanComponent::MsPacmanComponent(bae::GameObject& owner) :
    Component{ owner },
    m_MsPacmanState{ nullptr }
{
    auto font = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 32);
    m_Owner->AddComponent<bae::TextComponent>(*m_Owner, "Default", font, bae::Utils::Color::Gray);
    m_Owner->AddComponent<LifeComponent>(*m_Owner, 4);

    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, "Textures/Characters/MsPacman.png",
                                                SDL_FRect(0, 0, 48, 64), 3, 12);

    m_Owner->AddComponent<ScoreComponent>(*m_Owner);
    auto scoreComponent = m_Owner->GetComponent<ScoreComponent>();
    scoreComponent->SetScore(100);

    m_MsPacmanState = std::make_unique<States::MsPacmanIdle>(*m_Owner);
    m_MsPacmanState->OnEnter();
}


void MsPacmanComponent::Update()
{
    UpdateToNewState(m_MsPacmanState->Update());

    if(const auto lifeComponent = m_Owner->GetComponent<LifeComponent>(); lifeComponent)
    {
        if(!lifeComponent->IsAlive())
        {
            UpdateToNewState(std::make_unique<States::MsPacmanDying>(*m_Owner));
        }
    }
}

States::EntityState* MsPacmanComponent::GetState() const
{
    return m_MsPacmanState.get();
}

void MsPacmanComponent::Notify(const unsigned int eventHash, bae::Subject*, const std::any& eventData)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
        case Events::GameOver:
        case Events::LevelWon:
        case Events::LevelLost:
        case Events::GhostDied:
        case Events::BeginLevel:
        case Events::RestartLevel:
            break;
        case Events::ScoreChanged:
        {
            const auto scoreComp = m_Owner->GetComponent<ScoreComponent>();
            if(!scoreComp)
            {
                return;
            }

            std::cout << FUNCTION_NAME << " Score changed: " << scoreComp->GetScore() << '\n';
        }
        break;
        case Events::LivesChanged:
        case Events::InvincibilityChanged:
            break;
        case Events::Collision:
            HandleCollision(eventData);
            break;
        case Events::NoEvent:
            break;
    }
}

void MsPacmanComponent::UpdateToNewState(std::unique_ptr<States::EntityState> newState)
{
    // if dying, ignore new state
    if(dynamic_cast<States::MsPacmanDying*>(m_MsPacmanState.get()))
    {
        return;
    }

    if(!newState)
    {
        return;
    }

    m_MsPacmanState->OnExit();
    m_MsPacmanState = std::move(newState);
    m_MsPacmanState->OnEnter();
}

void MsPacmanComponent::HandleCollision(const std::any& eventData) const
{
    if(!eventData.has_value())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get EventData"));
    }

    auto otherHitbox = std::any_cast<HitboxComponent*>(eventData);
    if(!otherHitbox)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Invalid EventData GameObject!"));
    }

    // TODO: implement Checking for what got hit
    std::cout << FUNCTION_NAME << " Collision detected!" << '\n';

    bae::GameObject* otherObject = otherHitbox->GetGameObject();

    if(auto itemComp = otherObject->GetComponent<ItemComponent>())
    {
        HandleItemCollision(*itemComp);
    }
    else
    // else if(otherObject->HasComponent<EnemyComponent>())
    {
        HandleEnemyCollision(nullptr);
    }
}

void MsPacmanComponent::HandleItemCollision(ItemComponent& itemComponent) const
{
    bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_LevelManagersSceneName.data());
    if(!scene)
    {
        return;
    }

    auto sceneGameObjects           = scene->GetObjects();
    const auto levelManagerObjectIt = std::ranges::find_if(sceneGameObjects, [](auto& object)
    {
        return object->template HasComponent<LevelManagerComponent>();
    });

    if(levelManagerObjectIt == sceneGameObjects.end())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Get levelManagerComponent"));
    }

    const auto levelManagerComponent = (*levelManagerObjectIt)->GetComponent<LevelManagerComponent>();
    const LevelJson levelJson        = levelManagerComponent->GetCurrentLevel();
    const auto scoreMap              = levelJson.ScoreMap;

    auto getScore = [&](const ScoreTypes scoreType) -> int
    {
        const auto scoreIt = scoreMap.find(scoreType);
        if(scoreIt == scoreMap.end())
        {
            return 0;
        }

        return scoreIt->second;
    };

    int scoreGained{};
    switch(itemComponent.GetItemType())
    {
        case ItemType::PacDot:
            scoreGained = getScore(ScoreTypes::PacDot);
            break;
        case ItemType::PowerPellet:
            scoreGained = getScore(ScoreTypes::PowerPellet);
            break;
        case ItemType::Fruit:
            scoreGained = getScore(ScoreTypes::BonusItem);
            break;
    }


    const auto scoreComp = m_Owner->GetComponent<ScoreComponent>();
    scoreComp->SetScore(scoreGained);

    if(!scoreGained)
    {
        std::cout << FUNCTION_NAME << " Score gained, couldn't find score, score: " << scoreGained << '\n';
    }

    itemComponent.GetOwner()->Destroy();
}

void MsPacmanComponent::HandleEnemyCollision(bae::GameObject*) const
{
}



