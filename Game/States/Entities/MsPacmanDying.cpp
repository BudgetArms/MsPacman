#include "MsPacmanDying.hpp"

#include <iostream>

#include "Base/Events.hpp"
#include "Components/TextComponent.hpp"
#include "Core/EventQueue.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"
#include "Sounds/SoundSystem.hpp"

#include "Base/SoundAssets.hpp"
#include "Components/MsPacmanComponent.hpp"


using namespace Game::States;


void MsPacmanDying::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    bae::SoundSystem& soundSystem   = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID startSoundID = Game::Sounds::GetSoundId(Sounds::SoundAssets::PlayerDeath);

    const bae::ActiveSoundID playingSoundID = soundSystem.Play(startSoundID);
    soundSystem.SetVolume(playingSoundID, 1.f);


    const auto textComponent = m_GameObject->GetComponent<bae::TextComponent>();
    if(!textComponent)
    {
        return;
    }
    textComponent->SetColor(bae::Utils::Color::Red);
    textComponent->SetText(FUNCTION_NAME);

    bae::EventQueue::GetInstance().SendEvent(GetEventHash(Events::PlayerDied));
}

void MsPacmanDying::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

std::unique_ptr<EntityState> MsPacmanDying::Update()
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
            bae::EventQueue::GetInstance().SendEvent(GetEventHash(Events::GameOver));
            m_GameObject->Destroy();
        }
    }

    return nullptr;
}

void MsPacmanDying::HandleInput()
{
}
