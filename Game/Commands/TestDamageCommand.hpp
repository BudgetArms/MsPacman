#pragma once

#include "Commands/GameActorCommand.hpp"

namespace Game
{
    class TestDamageCommand : public bae::GameActorCommand
    {
    public:
        TestDamageCommand(bae::GameObject& actor);

        void Execute() override;
    };
}
