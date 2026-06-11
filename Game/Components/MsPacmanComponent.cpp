#include "MsPacmanComponent.hpp"

#include "Components/SpriteComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Core/GameObject.hpp"
#include "Core/Scene.hpp"
#include "Managers/ResourceManager.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Base/GhostKiller.hpp"
#include "Base/Level.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/LevelManagerComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/ScoreComponent.hpp"
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
    const auto scoreComponent = m_Owner->GetComponent<ScoreComponent>();
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
        case Events::PlayerChangedDirection:
            break;
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
            break;
        case Events::InvincibilityChanged:
            std::cout << "Invinciblity Changed\n\n\n";
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

    const auto otherHitbox = std::any_cast<HitboxComponent*>(eventData);
    if(!otherHitbox)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Invalid EventData GameObject!"));
    }

    bae::GameObject* otherObject = otherHitbox->GetGameObject();
    if(const auto itemComp = otherObject->GetComponent<ItemComponent>())
    {
        HandleItemCollision(*itemComp);
    }
    else if(IsEnemy(otherObject))
    {
        HandleEnemyCollision(otherObject);
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

    if(scoreGained == 0)
    {
        std::cout << FUNCTION_NAME << " Score gained, couldn't find score, score: " << scoreGained << '\n';
    }

    itemComponent.GetOwner()->Destroy();
}

void MsPacmanComponent::HandleEnemyCollision(bae::GameObject* gameObject) const
{
    const auto lifeComp = m_Owner->GetComponent<LifeComponent>();
    if(!lifeComp)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get LifeComponent!"));
    }

    if(lifeComp->IsInvincible())
    {
        GhostKiller* ghostKiller = GetEnemyClass(gameObject);
        ghostKiller->Kill();
    }
    else
    {
        std::cout << "Removed Life\n";
        lifeComp->RemoveLife();
    }
}

bool MsPacmanComponent::IsEnemy(bae::GameObject* gameObject)
{
    return GetEnemyClass(gameObject) != nullptr;
}

GhostKiller* MsPacmanComponent::GetEnemyClass(bae::GameObject* gameObject)
{
    auto& components         = gameObject->GetComponents();
    const auto foundKillerIt = std::ranges::find_if(components, [](auto& component)
    {
        return dynamic_cast<GhostKiller*>(component.get());
    });

    if(foundKillerIt == components.end())
    {
        return nullptr;
    }

    return dynamic_cast<GhostKiller*>(foundKillerIt->get());
}

