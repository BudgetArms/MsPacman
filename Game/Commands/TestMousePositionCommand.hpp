#pragma once

#include "Commands/Command.hpp"


namespace Game
{
    class TestMousePositionCommand final : public bae::Command
    {
    public:
        TestMousePositionCommand() = default;

        void Execute() override;
    };
}
