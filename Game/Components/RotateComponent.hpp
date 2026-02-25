#pragma once

#include "Components/Component.h"


namespace Game
{
    class RotateComponent : public bae::Component
    {
    public:
        explicit RotateComponent(bae::GameObject& owner, const glm::vec2& centerPosition, const float radius, const float angularSpeed);

        void Update() override;

    private:
        glm::vec2 m_CenterPosition;
        float m_Radius;
        float m_AngularSpeed;

        float m_Angle{};

    };

}
