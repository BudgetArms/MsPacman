#include "HitboxComponent.hpp"

#include "Core/Utils.hpp"


using namespace Game;


HitboxComponent::HitboxComponent(bae::GameObject& owner, const glm::vec2& dimensions, const glm::vec2& offsetPosition) :
    Component(owner),
    m_Dimensions{ dimensions },
    m_OffsetPosition{ offsetPosition }
{
}


void HitboxComponent::Render() const
{
    if(!m_bIsVisible)
    {
        return;
    }

    bae::Utils::DrawRect(GetHitbox(), m_Color);
}

SDL_FRect HitboxComponent::GetHitbox() const
{
    const glm::vec2 gameObjectPos = m_Owner->GetWorldLocation();
    const glm::vec2 hitboxPos     = gameObjectPos + m_OffsetPosition;

    const SDL_FRect hitboxRect
    {
        hitboxPos.x,
        hitboxPos.y,
        m_Dimensions.x,
        m_Dimensions.y,
    };

    return hitboxRect;
}

void HitboxComponent::SetDimensions(const glm::vec2& dimensions)
{
    m_Dimensions = dimensions;
}

void HitboxComponent::SetOffset(const glm::vec2& offsetPosition)
{
    m_OffsetPosition = offsetPosition;
}

void HitboxComponent::SetColor(const bae::Utils::Color& color)
{
    m_Color = color;
}

void HitboxComponent::SetVisibility(const bool visibility)
{
    m_bIsVisible = visibility;
}
