#include "BlinkyComponent.hpp"

#include "States/Entities/BlinkyChasing.hpp"
#include "States/Entities/GhostStates.hpp"


using namespace Game;


BlinkyComponent::BlinkyComponent(bae::GameObject& owner) :
    Component(owner)
{
    m_BlinkyState = std::make_unique<States::BlinkyChasing>(*m_Owner);
}

BlinkyComponent::~BlinkyComponent()
{
}

void BlinkyComponent::Update()
{
    UpdateToNewState(m_BlinkyState->Update());
}

States::EntityState* BlinkyComponent::GetState() const
{
    return m_BlinkyState.get();
}

void BlinkyComponent::Kill()
{
    m_bPendingKilled = true;
}

void BlinkyComponent::UpdateToNewState(std::unique_ptr<States::EntityState> newState)
{
    if(!newState && !m_bPendingKilled)
    {
        return;
    }

    m_BlinkyState->OnExit();

    if(m_bPendingKilled)
    {
        m_bPendingKilled = false;
        newState         = std::make_unique<States::BlinkyChasing>(*m_Owner);
    }

    m_BlinkyState = std::move(newState);
    m_BlinkyState->OnEnter();
}


