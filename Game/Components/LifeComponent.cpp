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
        // TODO: call removed life event

        return;
    }

    // TODO: call death event
    m_bIsAlive = false;
}

void LifeComponent::RemoveAllLives()
{
    if(!IsAlive())
    {
        return;
    }

    m_Lives    = 0;
    m_bIsAlive = false;
    // TODO: call death event
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
    // TODO: Call updated lives
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
    // TODO: Call updated lives
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

    // TODO: Call updated invincibility
}

bool LifeComponent::IsAlive() const
{
    return m_bIsAlive;
}

