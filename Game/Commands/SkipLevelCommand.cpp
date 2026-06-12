#include "SkipLevelCommand.hpp"

using namespace Game;

SkipLevelCommand::SkipLevelCommand(bae::GameObject& owner) :
    GameActorCommand(owner)
{
    m_LevelManagerComponent = GetActor()->GetComponent<LevelManagerComponent>();
}

void SkipLevelCommand::Execute()
{
    if(!GetActor() || GetActor()->IsMarkedForDeletion())
    {
        return;
    }
    if(!m_LevelManagerComponent)
    {
        return;
    }

    m_LevelManagerComponent->SkipLevel();
}
