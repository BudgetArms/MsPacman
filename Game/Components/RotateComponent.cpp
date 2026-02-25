#include "RotateComponent.hpp"

#include <valarray>

#include "ext/scalar_constants.hpp"
#include "Singletons/GameTime.h"


Game::RotateComponent::RotateComponent(bae::GameObject &owner, const glm::vec2& centerPosition, const float radius,
                                       const float angularSpeed) :
    Component(owner),
    m_CenterPosition{centerPosition},
    m_Radius{ radius },
    m_AngularSpeed{ angularSpeed }
{
}

void Game::RotateComponent::Update()
{
    m_Angle += m_AngularSpeed * bae::GameTime::GetInstance().GetDeltaTime();

    const glm::vec2 offsetPosition{ m_Radius * glm::cos(m_Angle), m_Radius * glm::sin(m_Angle) };

    m_Owner->SetLocalLocation(m_CenterPosition + offsetPosition);
}

