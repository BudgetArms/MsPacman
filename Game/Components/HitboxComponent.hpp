#pragma once

#include "Components/Component.hpp"
#include "Core/Subject.hpp"
#include "Core/Utils.hpp"


namespace Game
{
    class HitboxComponent final : public bae::Component, public bae::Subject
    {
    public:
        explicit HitboxComponent(bae::GameObject& owner, const glm::vec2& dimensions, const glm::vec2& offsetPosition);

        void Render() const override;

        [[nodiscard]] SDL_FRect GetHitbox() const;

        void SetDimensions(const glm::vec2& dimensions);
        void SetOffset(const glm::vec2& offsetPosition);
        void SetColor(const bae::Utils::Color& color);
        void SetVisibility(bool visibility);

    private:
        glm::vec2 m_Dimensions{};
        glm::vec2 m_OffsetPosition{};
        bae::Utils::Color m_Color{ bae::Utils::Color::Green };

        bool m_bIsVisible{ false };
    };
}
