#pragma once

#include "Commands/GameActorCommand.hpp"


namespace Game
{
    class TestDamageCommand : public bae::GameActorCommand
    {
    public:
        explicit TestDamageCommand(bae::GameObject& actor);

        void Execute() override;
    };
}
