#pragma once
#include "../Base/SmartpointerHelpers.hpp"
#include "Commands/GameActorCommand.h"


namespace std::filesystem
{
    class path;
}

class MIX_Init;

namespace Game
{
    class TestSoundCommand : public bae::GameActorCommand
    {
    public:
        TestSoundCommand(bae::GameObject& owner, const std::filesystem::path& soundPath);

        void Execute() override;

    private:
        std::unique_ptr<MIX_Mixer, Game::MIX_MixerDeletor> m_Mixer{};
        std::unique_ptr<MIX_Audio, MIX_AudioDeletor> m_Audio{};
        std::unique_ptr<MIX_Track, MIX_TrackDeletor> m_Track{};
        bool m_bIsMixerValid{};
    };
}
