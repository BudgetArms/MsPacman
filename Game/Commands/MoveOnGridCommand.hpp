#pragma once
#include "Commands/GameActorCommand.h"

#include "../Base/DirectionEnum.hpp"

namespace Game
{
    class MoveOnGridCommand : public bae::GameActorCommand
    {
    public:
        MoveOnGridCommand(bae::GameObject& owner, Direction direction);

        void Execute() override;

    private:
        Direction m_Direction;

    };
}
