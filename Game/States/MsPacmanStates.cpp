#include "MsPacmanStates.hpp"

#include <iostream>

#include "Core/GameObject.h"
#include "Core/HelperFunctions.h"
#include "Core/ServiceLocator.h"
#include "Singletons/GameTime.h"

#include "../Base/SoundEvents.hpp"
#include "../Components/MsPacmanComponent.hpp"


using namespace Game::States;


MsPacmanState::MsPacmanState(bae::GameObject& gameObject) :
    m_GameObject{ &gameObject }
{
}

// Idle
void MsPacmanIdle::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    bae::SoundSystem& soundSystem   = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID startSoundID = Game::Sounds::GetSoundId(Sounds::SoundEvents::BeepSound);

    m_PlayingActiveSoundId = soundSystem.Play(startSoundID);
    soundSystem.SetVolume(m_PlayingActiveSoundId, 2.f);
    soundSystem.Loop(m_PlayingActiveSoundId);
}

void MsPacmanIdle::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';

    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();
    soundSystem.UnLoop(m_PlayingActiveSoundId);
}

void MsPacmanIdle::Update()
{
    if(m_AccumulatedTime == 0.0f)
    {
        std::cout << FUNCTION_NAME << '\n';
    }
    m_AccumulatedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime >= m_TimeSwitch)
    {
        if(m_GameObject->HasComponent<MsPacmanComponent>())
        {
            auto msPacmanComp = m_GameObject->GetComponent<MsPacmanComponent>();
            msPacmanComp->SetState(std::make_unique<MsPacmanMoving>(*m_GameObject));
        }
    }
}


// Moving
void MsPacmanMoving::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';
}

void MsPacmanMoving::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

void MsPacmanMoving::Update()
{
    if(m_AccumulatedTime == 0.0f)
    {
        std::cout << FUNCTION_NAME << '\n';
    }

    m_AccumulatedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime >= m_TimeSwitch)
    {
        if(m_GameObject->HasComponent<MsPacmanComponent>())
        {
            const auto msPacmanComp = m_GameObject->GetComponent<MsPacmanComponent>();
            msPacmanComp->SetState(std::make_unique<MsPacmanDying>(*m_GameObject));
        }
    }
}


// Invincible
void MsPacmanInvincible::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';
}

void MsPacmanInvincible::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

void MsPacmanInvincible::Update()
{
    std::cout << FUNCTION_NAME << '\n';
}


// Hit
void MsPacmanHit::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';
}

void MsPacmanHit::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

void MsPacmanHit::Update()
{
    std::cout << FUNCTION_NAME << '\n';
}


// Dying
void MsPacmanDying::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    bae::SoundSystem& soundSystem   = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID startSoundID = Game::Sounds::GetSoundId(Sounds::SoundEvents::PlayerDeath);

    const bae::ActiveSoundID playingSoundID = soundSystem.Play(startSoundID);
    soundSystem.SetVolume(playingSoundID, 2.f);
}

void MsPacmanDying::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

void MsPacmanDying::Update()
{
    if(m_AccumulatedTime == 0.0f)
    {
        std::cout << FUNCTION_NAME << '\n';
    }

    m_AccumulatedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime >= m_TimeSwitch)
    {
        if(m_GameObject->HasComponent<MsPacmanComponent>())
        {
            const auto msPacmanComp = m_GameObject->GetComponent<MsPacmanComponent>();
            msPacmanComp->SetState(nullptr);

            m_GameObject->Destroy();
        }
    }
}

