#pragma once

#include "Commands/GameActorCommand.hpp"

#include "../Base/SoundAssets.hpp"

class MIX_Init;

namespace Game
{
    class TestSoundCommand : public bae::GameActorCommand
    {
    public:
        TestSoundCommand(bae::GameObject& owner, Sounds::SoundAssets soundEvent);

        void Execute() override;

    private:
        Sounds::SoundAssets m_SoundEvent;
    };
}
