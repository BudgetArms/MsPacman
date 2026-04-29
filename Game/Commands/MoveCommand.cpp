#include "MoveCommand.hpp"

#include "../Base/DirectionEnum.hpp"
#include "Singletons/GameTime.h"
#include "Singletons/Singleton.h"


Game::MoveCommand::MoveCommand(bae::GameObject& owner, const Direction direction, const float speed) :
    GameActorCommand(owner),
    m_Direction{ direction },
    m_Speed{ speed }
{
}

void Game::MoveCommand::Execute()
{
    const float deltaSpeed = m_Speed * bae::GameTime::GetInstance().GetDeltaTime();
    switch(m_Direction)
    {
        case Direction::Down:
        {
            GetActor()->AddLocation({ 0.f, +deltaSpeed });
        }
        break;
        case Direction::Up:
        {
            GetActor()->AddLocation({ 0.f, -deltaSpeed });
        }
        break;
        case Direction::Left:
        {
            GetActor()->AddLocation({ -deltaSpeed, 0.f });
        }
        break;
        case Direction::Right:
        {
            GetActor()->AddLocation({ +deltaSpeed, 0.f });
        }
        break;
    }
}
