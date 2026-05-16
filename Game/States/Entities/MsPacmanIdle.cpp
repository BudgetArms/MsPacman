#include "MsPacmanIdle.hpp"

#include <iostream>

#include "Components/TextComponent.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"
#include "Sounds/SoundSystem.hpp"

#include "MsPacmanMoving.hpp"
#include "../Game/Base/SoundAssets.hpp"
#include "../Game/Components/MsPacmanComponent.hpp"


using namespace Game::States;


void MsPacmanIdle::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    bae::SoundSystem& soundSystem   = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID startSoundID = Game::Sounds::GetSoundId(Sounds::SoundAssets::BeepSound);

    m_PlayingActiveSoundId = soundSystem.Play(startSoundID);
    soundSystem.SetVolume(m_PlayingActiveSoundId, 2.f);
    soundSystem.Loop(m_PlayingActiveSoundId);

    m_GameObject->AddLocation({ -300, 0 });

    auto textComponent = m_GameObject->GetComponent<bae::TextComponent>();
    if(!textComponent)
    {
        return;
    }
    textComponent->SetColor(bae::Utils::Color::Blue);
    textComponent->SetText(FUNCTION_NAME);
}

void MsPacmanIdle::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';

    const auto textComponent = m_GameObject->GetComponent<bae::TextComponent>();
    if(!textComponent)
    {
        return;
    }
    textComponent->SetColor(bae::Utils::Color::Blue);
    textComponent->SetText(FUNCTION_NAME);

    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();
    soundSystem.UnLoop(m_PlayingActiveSoundId);
}

std::unique_ptr<EntityState> MsPacmanIdle::Update()
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

void MsPacmanIdle::HandleInput()
{
}
