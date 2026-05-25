#include "ToggleMuteAllSoundsCommand.hpp"

#include "Commands/Command.hpp"
#include "Core/ServiceLocator.hpp"


using namespace Game;


ToggleMuteAllSoundsCommand::ToggleMuteAllSoundsCommand() :
    Command()
{
}

void ToggleMuteAllSoundsCommand::Execute()
{
    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();

    if(m_bIsMuted)
    {
        soundSystem.UnMuteAllSounds();
    }
    else
    {
        soundSystem.MuteAllSounds();
    }

    m_bIsMuted = !m_bIsMuted;
}
