#pragma once

#include "../Base/SmartpointerHelpers.hpp"
#include "Commands/GameActorCommand.h"

#include "../Base/SoundEvents.hpp"

class MIX_Init;

namespace Game
{
    class TestSoundCommand : public bae::GameActorCommand
    {
    public:
        TestSoundCommand(bae::GameObject& owner, Sounds::SoundEvents soundEvent);

        void Execute() override;

    private:
        Sounds::SoundEvents m_SoundEvent;
    };
}
