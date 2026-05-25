#pragma once


#include "Commands/Command.hpp"


namespace Game
{
    class ToggleMuteAllSoundsCommand final : public bae::Command
    {
    public:
        ToggleMuteAllSoundsCommand();

        void Execute() override;

    private:
        bool m_bIsMuted{ false };
    };
}
