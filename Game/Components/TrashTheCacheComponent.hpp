#pragma once

#include "Core/GameObject.h"
#include "Components/ImguiComponent.h"


namespace Game
{
    class TrashTheCacheComponent final : public bae::ImguiComponent
    {
    public:
        explicit TrashTheCacheComponent(bae::GameObject& owner, bool bIsExercise2);
        ~TrashTheCacheComponent() override = default;

        TrashTheCacheComponent(const TrashTheCacheComponent& other)            = delete;
        TrashTheCacheComponent(TrashTheCacheComponent&& other)                 = delete;
        TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
        TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other)      = delete;

        void RenderGUI() override;

    private:
        void TimingExercise1();
        void TimingExercise2();
        void TimingExercise2Alt();


        bool m_bIsExercise2{ false };
        int m_NrSamples;

        const int m_BufferSize{ static_cast<int>(std::pow(2, 26)) };

        std::vector<float> m_TimingsExercise1;
        std::vector<float> m_TimingsExercise2;
        std::vector<float> m_TimingsExercise2Alt;
    };
}

