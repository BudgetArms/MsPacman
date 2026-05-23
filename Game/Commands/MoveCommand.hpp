#pragma once

#include "Commands/GameActorCommand.hpp"

#include "Base/DirectionEnum.hpp"
#include "Base/Events.hpp"
#include "Core/EventListener.hpp"
#include "Core/HelperFunctions.hpp"


namespace Game
{
    class MoveCommand final : public bae::GameActorCommand, public bae::EventListener
    {
    public:
        MoveCommand(bae::GameObject& owner, Direction direction, float speed);

        void Execute() override;

        void HandleEvent(const unsigned int eventHash) override
        {
            const Events event = GetEvent(eventHash);

            std::cout << "MoveCommand::HandleEvent: ";
            switch(event)
            {
                case Events::GamePaused:
                case Events::GameResumed:
                case Events::PlayerDied:
                    break;
                case Events::GameOver:
                    std::cout << " GameOver" << '\n';
                    break;
                case Events::GameWon:
                case Events::GhostDied:
                    break;
                case Events::BeginLevel:
                    std::cout << " GameOver" << '\n';
                    break;
                case Events::RestartLevel:
                case Events::ScoreChanged:
                case Events::NoEvent:
                case Events::LivesChanged:
                case Events::InvincibilityChanged:
                default:
                    break;
            }
        }

    private:
        Direction m_Direction;
        float m_Speed;
    };
}
