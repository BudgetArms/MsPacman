#include "MsPacmanInvincible.hpp"

#include <iostream>

#include "Components/TextComponent.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"
#include "Sounds/SoundSystem.hpp"

#include "Base/SoundAssets.hpp"
#include "Components/MsPacmanComponent.hpp"
#include "States/Entities/MsPacmanMoving.hpp"


using namespace Game::States;


void MsPacmanInvincible::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    bae::SoundSystem& soundSystem   = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID startSoundID = Game::Sounds::GetSoundId(Sounds::SoundAssets::PlayerDeath);

    const bae::ActiveSoundID playingSoundID = soundSystem.Play(startSoundID);
    soundSystem.SetVolume(playingSoundID, 0.5f);
}

void MsPacmanInvincible::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

std::unique_ptr<EntityState> MsPacmanInvincible::Update()
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
            return std::make_unique<MsPacmanMoving>(*m_GameObject);
        }
    }

    return nullptr;
}

void MsPacmanInvincible::HandleInput()
{
}
