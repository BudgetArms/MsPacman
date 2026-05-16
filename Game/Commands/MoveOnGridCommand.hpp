#pragma once

#include "Commands/GameActorCommand.hpp"

#include "../Base/DirectionEnum.hpp"

namespace Game
{
    class MoveOnGridCommand final : public bae::GameActorCommand
    {
    public:
        MoveOnGridCommand(bae::GameObject& owner, Direction direction);

        void Execute() override;

    private:
        Direction m_Direction;
    };
}

