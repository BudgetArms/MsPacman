#include "MsPacmanInvincible.hpp"

#include <iostream>

#include "MsPacmanIdle.hpp"
#include "Components/TextComponent.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"
#include "Sounds/SoundSystem.hpp"

#include "../Game/Base/SoundAssets.hpp"
#include "../Game/Components/MsPacmanComponent.hpp"


using namespace Game::States;


void MsPacmanInvincible::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    bae::SoundSystem& soundSystem   = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID startSoundID = Game::Sounds::GetSoundId(Sounds::SoundAssets::PlayerDeath);

    const bae::ActiveSoundID playingSoundID = soundSystem.Play(startSoundID);
    soundSystem.SetVolume(playingSoundID, 2.f);


    const auto textComponent = m_GameObject->GetComponent<bae::TextComponent>();
    if(!textComponent)
    {
        return;
    }

    textComponent->SetColor(bae::Utils::Color::Red);
    textComponent->SetText(FUNCTION_NAME);
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
            return std::make_unique<MsPacmanIdle>(*m_GameObject);
        }
    }

    return nullptr;
}

void MsPacmanInvincible::HandleInput()
{
}
