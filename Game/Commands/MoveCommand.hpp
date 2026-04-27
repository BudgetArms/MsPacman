#pragma once
#include "Commands/GameActorCommand.h"

#include "../Base/DirectionEnum.hpp"


namespace Game
{
    class MoveCommand : public bae::GameActorCommand
    {
    public:
        MoveCommand(bae::GameObject& owner, Direction direction, float speed);

        void Execute() override;

    private:
        Direction m_Direction;
        float m_Speed;
    };
}
