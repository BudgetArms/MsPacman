#include "TestSoundCommand.hpp"

#include <filesystem>

#include "Core/HelperFunctions.h"
#include "Core/ServiceLocator.h"
#include "Sounds/SoundSystem.h"
#include "Managers/ResourceManager.h"

#include "../Base/SoundEvents.hpp"


Game::TestSoundCommand::TestSoundCommand(bae::GameObject& owner, Sounds::SoundEvents soundEvent) :
    GameActorCommand(owner),
    m_SoundEvent{ soundEvent }
{
}

void Game::TestSoundCommand::Execute()
{
    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID soundId    = Sounds::GetSoundId(m_SoundEvent);
    const bae::ActiveSoundID asId = soundSystem.Play(soundId);
    if(asId.ID == -1)
    {
        std::cout << FUNCTION_NAME << " Failed to play sound" << '\n';
    }
}
