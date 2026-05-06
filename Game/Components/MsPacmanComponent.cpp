#include "MsPacmanComponent.hpp"

#include "Components/TextComponent.h"
#include "Core/GameObject.h"
#include "Managers/ResourceManager.h"

#include "../States/MsPacmanStates.hpp"


Game::MsPacmanComponent::MsPacmanComponent(bae::GameObject& owner) :
    Component{ owner },
    m_MsPacmanState{ nullptr }
{
    auto font = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 32);
    m_Owner->AddComponent<bae::TextComponent>(*m_Owner, "Default", font, bae::Utils::Color::Gray);

    m_MsPacmanState = std::make_unique<States::MsPacmanIdle>(*m_Owner);
    m_MsPacmanState->OnEnter();
}

Game::MsPacmanComponent::~MsPacmanComponent()
{
}

void Game::MsPacmanComponent::Update()
{
    if(m_MsPacmanState)
    {
        m_MsPacmanState->Update();
    }
}


void Game::MsPacmanComponent::SetState(std::unique_ptr<States::MsPacmanState> state)
{
    if(m_MsPacmanState)
    {
        m_MsPacmanState->OnExit();
    }

    m_MsPacmanState = nullptr;

    if(state)
    {
        m_MsPacmanState = std::move(state);
        m_MsPacmanState->OnEnter();
    }
}

