#include "MoveCommand.hpp"

#include "../Base/DirectionEnum.hpp"
#include "Singletons/GameTime.h"
#include "Singletons/Singleton.h"


Game::MoveCommand::MoveCommand(bae::GameObject& owner, Game::Direction direction, float speed) :
    GameActorCommand(owner),
    m_Direction{ direction },
    m_Speed{ speed }
{
}

void Game::MoveCommand::Execute()
{
    switch (m_Direction)
    {
        case Direction::Down:
        {
            GetActor()->AddLocation({0.f, +m_Speed * bae::GameTime::GetInstance().GetDeltaTime()});
        } break;
        case Direction::Up:
        {
            GetActor()->AddLocation({0.f, -m_Speed * bae::GameTime::GetInstance().GetDeltaTime()});
        } break;
        case Direction::Left:
        {
            GetActor()->AddLocation({-m_Speed * bae::GameTime::GetInstance().GetDeltaTime(), 0.f});
        } break;
        case Direction::Right:
        {
            GetActor()->AddLocation({+m_Speed * bae::GameTime::GetInstance().GetDeltaTime(), 0.f});
        } break;
    }


}
