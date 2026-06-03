#pragma once

#include <unordered_map>

#include "Sounds/SoundSystem.hpp"


namespace Game::Sounds
{
    enum class SoundAssets
    {
        Credit,
        EatDot,
        EatEnergizer,
        EatFruit,
        EatGhost,
        ExtraLife,
        Eyes,
        EyesFirstLoop,
        Fright,
        FruitBounce,
        IntermissionTune1,
        IntermissionTune1_Bump,
        IntermissionTune2,
        IntermissionTune3,
        PlayerDeath,
        StartGame,
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

