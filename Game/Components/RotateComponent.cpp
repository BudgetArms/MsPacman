#include "RotateComponent.hpp"

#include <numbers>

#include "Singletons/GameTime.h"


Game::RotateComponent::RotateComponent(bae::GameObject& owner, const float radius, const float angularSpeed) :
    Component(owner),
    m_Radius{ radius },
    m_AngularSpeed{ angularSpeed }
{
}


void Game::RotateComponent::Update()
{
    // Uses parent position to know the center position
    if(!m_Owner->GetParent())
    {
        if(!m_bHasPrintedMessage)
        {
            // Verbose
            std::cout << "RotateComponent Warning: " << m_Owner->GetName() << " has no owner" << '\n';

            m_bHasPrintedMessage = true;
        }

        return;
    }

    m_Angle += m_AngularSpeed * bae::GameTime::GetInstance().GetDeltaTime();
    m_Angle = std::fmodf(m_Angle, 2 * static_cast<float>(std::numbers::pi));

    const glm::vec2 offsetPosition{ m_Radius * glm::cos(m_Angle), m_Radius * glm::sin(m_Angle) };

    m_Owner->SetLocalLocation(offsetPosition);
}

