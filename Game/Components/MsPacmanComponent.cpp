#include "MsPacmanComponent.hpp"

#include "ScoreComponent.hpp"
#include "Base/Events.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Core/GameObject.hpp"
#include "Managers/ResourceManager.hpp"

#include "Components/LifeComponent.hpp"
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

void MsPacmanComponent::Notify(const unsigned int eventHash, bae::Subject*)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
        case Events::GameOver:
        case Events::GameWon:
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
        case Events::GamePaused:
        case Events::GameResumed:
        case Events::NoEvent:
        default:;
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



