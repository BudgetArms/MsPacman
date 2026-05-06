#pragma once

#include <unordered_map>

#include "Sounds/SoundSystem.h"


namespace Game::Sounds
{
    enum class SoundEvents
    {
        GameplayMusic,
        BeepSound,
        PlayerDeath,
    };


    inline std::unordered_map<SoundEvents, bae::SoundID> g_sSoundEvents{};

    static bae::SoundID GetSoundId(const SoundEvents& event)
    {
        const auto it = g_sSoundEvents.find(event);
        if(it == g_sSoundEvents.end())
        {
            return bae::SoundID{ .ID = -1 };
        }

        return it->second;
    }
}

