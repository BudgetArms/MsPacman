#include "TestMousePositionCommand.hpp"

#include <iostream>
#include <SDL3/SDL_mouse.h>

#include "Components/LevelGridComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;


void TestMousePositionCommand::Execute()
{
    float x{};
    float y{};
    SDL_GetMouseState(&x, &y);

    std::cout << "MousePos X: " << static_cast<int>(x) << ", Y: " << static_cast<int>(y) << '\n';

    const auto levelGridComp = LevelManager::GetInstance().GetLevelGridComponent();
    if(!levelGridComp)
    {
        return;
    }

    const auto gridPos = levelGridComp->GetGridPosition({ x, y });
    if(!levelGridComp->IsValidGridPosition(gridPos))
    {
        std::cout << "not valid" << '\n';
    }

    std::cout << "GridPos Column: " << gridPos.Column << ", Row: " << gridPos.Row << '\n';
}
