#pragma once

#include "Commands/GameActorCommand.hpp"

#include "Base/DirectionEnum.hpp"
#include "Components/GridMovementComponent.hpp"

namespace Game
{
    class MoveOnGridCommand final : public bae::GameActorCommand
    {
    public:
        MoveOnGridCommand(bae::GameObject& owner, Direction direction);

        void Execute() override;

    private:
        GridMovementComponent* m_GridMovementComponent;
        Direction m_Direction{};
    };
}

