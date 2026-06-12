#pragma once

#include <algorithm>
#include <iostream>

#include "Core/HelperFunctions.hpp"


namespace Game
{
    enum class Events
    {
        PlayerDied,
        ChangedDirection,
        GameOver,
        LevelWon,
        LevelLost,
        GhostDied,
        BeginLevel,
        RestartLevel,
        ScoreChanged,
        LivesChanged,
        InvincibilityChanged,
        Collision,
        NoEvent,
    };


    inline const std::vector<std::pair<Events, unsigned int>> g_EventsMap
    {
        { Events::PlayerDied, bae::HashSDBM("PlayerDied") },
        { Events::ChangedDirection, bae::HashSDBM("ChangedDirection") },
        { Events::GameOver, bae::HashSDBM("GameOver") },
        { Events::LevelWon, bae::HashSDBM("LevelWon") },
        { Events::LevelLost, bae::HashSDBM("LevelLost") },
        { Events::GhostDied, bae::HashSDBM("GhostDied") },
        { Events::BeginLevel, bae::HashSDBM("BeginLevel") },
        { Events::RestartLevel, bae::HashSDBM("RestartLevel") },
        { Events::ScoreChanged, bae::HashSDBM("ScoreChanged") },
        { Events::LivesChanged, bae::HashSDBM("LivesChanged") },
        { Events::InvincibilityChanged, bae::HashSDBM("InvincibilityChanged") },
        { Events::Collision, bae::HashSDBM("Collision") },
        { Events::NoEvent, bae::HashSDBM("NoEvent") },
    };


    constexpr unsigned int GetEventHash(const Events event)
    {
        const auto eventPairIt = std::ranges::find_if(g_EventsMap,
                                                      [&](auto pair)
                                                      {
                                                          const auto& eventPair = pair.first;
                                                          return event == eventPair;
                                                      });

        if(eventPairIt == g_EventsMap.end())
        {
            std::cout << FUNCTION_NAME << " Failed! Not Implemented" << '\n';
            return bae::HashSDBM("NoEvent");
        }

        return eventPairIt->second;
    }


    constexpr Events GetEvent(unsigned int eventHash)
    {
        const auto eventPairIt = std::ranges::find_if(g_EventsMap,
                                                      [&](auto pair)
                                                      {
                                                          const auto& eventHashPair = pair.second;
                                                          return eventHash == eventHashPair;
                                                      });

        if(eventPairIt == g_EventsMap.end())
        {
            std::cout << FUNCTION_NAME << " Failed! Not Implemented" << '\n';
            return Events::NoEvent;
        }

        return eventPairIt->first;
    }
}

