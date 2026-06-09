#include "LifeComponent.hpp"

#include "Core/HelperFunctions.hpp"


using namespace Game;


LifeComponent::LifeComponent(bae::GameObject& owner, const int maxLives) :
    Component(owner),
    Subject(owner),
    m_Lives{ maxLives },
    m_MaxLives{ maxLives }
{
    if(m_MaxLives <= 0)
    {
        std::cout << FUNCTION_NAME << " Health is less than 0" << '\n';

        m_Lives    = 0;
        m_MaxLives = 0;
        m_bIsAlive = false;
    }
}

void LifeComponent::AddLife()
{
    if(!IsAlive())
    {
        return;
    }

    ++m_Lives;
    SendEventToObservers(Events::LivesChanged);
}

void LifeComponent::RemoveLife()
{
    if(!IsAlive() || IsInvincible())
    {
        return;
    }

    --m_Lives;

    if(m_Lives > 0)
    {
        SendEventToObservers(Events::LivesChanged);
        return;
    }

    m_bIsAlive = false;
    SendEventToObservers(Events::LivesChanged);
    SendEventToObservers(Events::PlayerDied);
}

void LifeComponent::RemoveAllLives()
{
    if(!IsAlive())
    {
        return;
    }

    m_Lives    = 0;
    m_bIsAlive = false;
    SendEventToObservers(Events::LivesChanged);
    SendEventToObservers(Events::PlayerDied);
}

int LifeComponent::GetLives() const
{
    return m_Lives;
}

void LifeComponent::SetLives(const int lives)
{
    if(!IsAlive())
    {
        return;
    }
    if(lives < 0)
    {
        std::cout << FUNCTION_NAME << " Failed! lives is less than 0" << '\n';
        return;
    }

    m_Lives = lives;
    SendEventToObservers(Events::LivesChanged);
}

int LifeComponent::GetMaxLives() const
{
    return m_MaxLives;
}

void LifeComponent::SetMaxLives(const int maxLives)
{
    if(!IsAlive())
    {
        return;
    }
    if(maxLives <= 0)
    {
        std::cout << FUNCTION_NAME << " Failed! MaxLives is equal or less than 0" << '\n';
        return;
    }

    m_MaxLives = maxLives;
    SendEventToObservers(Events::LivesChanged);
}

bool LifeComponent::IsInvincible() const
{
    return m_bIsInvincible;
}

void LifeComponent::SetInvincibility(const bool isInvincible)
{
    if(!IsAlive())
    {
        return;
    }

    m_bIsInvincible = isInvincible;

    SendEventToObservers(Events::InvincibilityChanged);
}

bool LifeComponent::IsAlive() const
{
    return m_bIsAlive;
}

void LifeComponent::SendEventToObservers(const Events event)
{
    NotifyObservers(GetEventHash(event));
}

