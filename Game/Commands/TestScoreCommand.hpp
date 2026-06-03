#pragma once

#include "Commands/GameActorCommand.hpp"


namespace Game
{
    class TestScoreCommand : public bae::GameActorCommand
    {
    public:
        explicit TestScoreCommand(bae::GameObject& actor);

        void Execute() override;
    };
}
