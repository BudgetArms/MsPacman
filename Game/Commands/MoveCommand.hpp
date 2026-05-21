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
            std::cout << "MoveCommand::HandleEvent: ";
            if(eventHash == GetEventHash(Events::GameOver))
            {
                std::cout << " GameOver" << '\n';
            }
            if(eventHash == GetEventHash(Events::BeginLevel))
            {
                std::cout << " BeginLevel" << '\n';
            }
            if(eventHash == GetEventHash(Events::GhostDied))
            {
                std::cout << " GhostDied" << '\n';
            }
            if(eventHash == GetEventHash(Events::PlayerDied))
            {
                std::cout << " PlayerDied" << '\n';
            }
        }

    private:
        Direction m_Direction;
        float m_Speed;
    };
}
