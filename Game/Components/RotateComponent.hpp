#pragma once

#include "Components/Component.h"


namespace Game
{
    class RotateComponent : public bae::Component
    {
    public:
        explicit RotateComponent(bae::GameObject& owner, float radius, float angularSpeed);

        void Update() override;

    private:
        float m_Radius;
        float m_AngularSpeed{ 1.f };

        float m_Angle{};

        bool m_bHasPrintedMessage{};
    };
}
