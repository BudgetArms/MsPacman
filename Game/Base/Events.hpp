#pragma once

#include <iostream>

#include "Core/HelperFunctions.hpp"


namespace Game
{
    enum class Events
    {
        GamePaused,
        GameResumed,
        PlayerDied,
        GameOver,
        GameWon,
        GhostDied,
        BeginLevel,
        RestartLevel,
    };


    consteval unsigned int GetEventHash(const Events event)
    {
        switch(event)
        {
            case Events::PlayerDied:
                return bae::HashSDBM("PlayerDied");
                break;
            case Events::GameOver:
                return bae::HashSDBM("GameOver");
                break;
            case Events::GameWon:
                return bae::HashSDBM("GameWon");
                break;
            case Events::GhostDied:
                return bae::HashSDBM("GameDied");
                break;
            case Events::BeginLevel:
                return bae::HashSDBM("BeginLevel");
                break;
            case Events::RestartLevel:
                return bae::HashSDBM("RestartLevel");
                break;
            case Events::GamePaused:
                return bae::HashSDBM("GamePaused");
                break;
            case Events::GameResumed:
                return bae::HashSDBM("GameResumed");
                break;
        }

        std::cout << FUNCTION_NAME << " Failed to get Proper Event Hash! Event not added" << '\n';
        return bae::HashSDBM("Unknown");
    }
}
