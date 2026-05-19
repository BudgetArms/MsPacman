#include "MoveOnGridCommand.hpp"

#include "Base/DirectionEnum.hpp"


Game::MoveOnGridCommand::MoveOnGridCommand(bae::GameObject& owner, const Direction direction) :
    GameActorCommand(owner),
    m_Direction{ direction }
{
    // Get the move command from the owner
}

void Game::MoveOnGridCommand::Execute()
{
    if(GetActor()->IsMarkedForDeletion())
    {
        return;
    }

    // TODO: Implement logic
    switch(m_Direction)
    {
        case Direction::Down:
        {
        }
        break;
        case Direction::Up:
        {
        }
        break;
        case Direction::Left:
        {
        }
        break;
        case Direction::Right:
        {
        }
        break;
    }
}
