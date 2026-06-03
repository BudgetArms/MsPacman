#pragma once

#include <glm/glm.hpp>

#include "Components/Component.hpp"
#include "Core/Observer.hpp"


namespace bae
{
    class Texture2D;
}

namespace Game
{
    class LifeComponent;

    class LifeDisplayComponent : public bae::Component, public bae::Observer
    {
    public:
        LifeDisplayComponent(bae::GameObject& owner, const glm::vec2& position,
                             std::shared_ptr<bae::Texture2D> lifeTexture);

        void Render() const override;


        void Notify(unsigned eventHash, bae::Subject* subject) override;


        glm::vec2 m_Position;
        glm::vec2 m_Margin{};

    private:
        std::shared_ptr<bae::Texture2D> m_LifeTexture;
        int m_Lives{};
    };
}
