#include "MoveOnGridCommand.hpp"

#include "Singletons/GameTime.h"

#include "../Game/Base/DirectionEnum.hpp"



Game::MoveOnGridCommand::MoveOnGridCommand(bae::GameObject& owner, Direction direction) :
    GameActorCommand(owner),
    m_Direction{ direction }
{
    // Get the move command from the owner
}

void Game::MoveOnGridCommand::Execute()
{
    switch (m_Direction)
    {
        case Direction::Down:
        {
        } break;
        case Direction::Up:
        {
        } break;
        case Direction::Left:
        {
        } break;
        case Direction::Right:
        {
        } break;
    }
}
