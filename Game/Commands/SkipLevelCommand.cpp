#include "SkipLevelCommand.hpp"

#include "Managers/LevelManager.hpp"


using namespace Game;


void SkipLevelCommand::Execute()
{
    LevelManager::GetInstance().SkipLevel();
}
