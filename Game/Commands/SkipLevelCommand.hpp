#pragma once

#include "Commands/GameActorCommand.hpp"


namespace Game
{
    class SkipLevelCommand final : public bae::Command
    {
    public:
        void Execute() override;
    };
}

