#pragma once

#include <unordered_map>

#include "Sounds/SoundSystem.h"


namespace Game::Sounds
{
    enum class SoundAssets
    {
        GameplayMusic,
        BeepSound,
        PlayerDeath,
    };


    inline std::unordered_map<SoundAssets, bae::SoundID> g_sSoundEvents{};

    [[maybe_unused]] static bae::SoundID GetSoundId(const SoundAssets& event)
    {
        const auto it = g_sSoundEvents.find(event);
        if(it == g_sSoundEvents.end())
        {
            return bae::SoundID{ .ID = -1 };
        }

        return it->second;
    }
}

