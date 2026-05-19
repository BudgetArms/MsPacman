#include "MsPacmanComponent.hpp"

#include "Components/TextComponent.hpp"
#include "Core/GameObject.hpp"
#include "Managers/ResourceManager.hpp"

#include "States/Entities/MsPacmanIdle.hpp"


Game::MsPacmanComponent::MsPacmanComponent(bae::GameObject& owner) :
    Component{ owner },
    m_MsPacmanState{ nullptr }
{
    auto font = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 32);
    m_Owner->AddComponent<bae::TextComponent>(*m_Owner, "Default", font, bae::Utils::Color::Gray);

    m_MsPacmanState = std::make_unique<States::MsPacmanIdle>(*m_Owner);
    m_MsPacmanState->OnEnter();
}


void Game::MsPacmanComponent::Update()
{
    if(!m_MsPacmanState)
    {
        return;
    }

    std::unique_ptr<States::EntityState> newState = m_MsPacmanState->Update();
    if(!newState)
    {
        return;
    }

    m_MsPacmanState->OnExit();
    m_MsPacmanState = std::move(newState);
    m_MsPacmanState->OnEnter();
}


