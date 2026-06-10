#include "MoveOnGridCommand.hpp"

#include "Base/DirectionEnum.hpp"
#include "Core/HelperFunctions.hpp"


Game::MoveOnGridCommand::MoveOnGridCommand(bae::GameObject& owner, const Direction direction) :
    GameActorCommand(owner),
    m_Direction{ direction }
{
    m_GridMovementComponent = GetActor()->GetComponent<GridMovementComponent>();
    if(!m_GridMovementComponent)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! No GridMovementComponent Found In Owner!"));
    }
}

void Game::MoveOnGridCommand::Execute()
{
    if(GetActor()->IsMarkedForDeletion())
    {
        return;
    }

    m_GridMovementComponent->SetDirection(m_Direction);
}
