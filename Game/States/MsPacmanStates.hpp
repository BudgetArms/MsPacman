#pragma once

#include "Core/State.h"
#include "Sounds/SoundStructs.h"

namespace bae
{
    class GameObject;
}

namespace Game::States
{
    class MsPacmanState : public bae::State
    {
    public:
        explicit MsPacmanState(bae::GameObject& gameObject);

        void OnEnter() override {};
        void OnExit() override {};
        void Update() override {};

    protected:
        bae::GameObject* m_GameObject;
    };

    class MsPacmanIdle final : public MsPacmanState
    {
    public:
        explicit MsPacmanIdle(bae::GameObject& gameObject)
            : MsPacmanState(gameObject) {}

        void OnEnter() override;
        void OnExit() override;
        void Update() override;

    private:
        bae::ActiveSoundID m_PlayingActiveSoundId{ -1 };

        // TODO: remove this code afterwards
        float m_AccumulatedTime{};
        static constexpr float m_TimeSwitch{ 3.f };
    };

    class MsPacmanMoving final : public MsPacmanState
    {
    public:
        explicit MsPacmanMoving(bae::GameObject& gameObject)
            : MsPacmanState(gameObject) {}

        void OnEnter() override;
        void OnExit() override;
        void Update() override;

    private:
        // TODO: remove this code afterwards
        float m_AccumulatedTime{};
        static constexpr float m_TimeSwitch{ 5.f };
    };

    class MsPacmanInvincible final : public MsPacmanState
    {
    public:
        explicit MsPacmanInvincible(bae::GameObject& gameObject)
            : MsPacmanState(gameObject) {}

        void OnEnter() override;
        void OnExit() override;
        void Update() override;
    };

    class MsPacmanHit final : public MsPacmanState
    {
    public:
        explicit MsPacmanHit(bae::GameObject& gameObject)
            : MsPacmanState(gameObject) {}

        void OnEnter() override;
        void OnExit() override;
        void Update() override;
    };

    class MsPacmanDying final : public MsPacmanState
    {
    public:
        explicit MsPacmanDying(bae::GameObject& gameObject)
            : MsPacmanState(gameObject) {}

        void OnEnter() override;
        void OnExit() override;
        void Update() override;

    private:
        float m_AccumulatedTime{};
        static constexpr float m_TimeSwitch{ 5.f };
    };
}
