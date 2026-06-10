#include "TestMousePositionCommand.hpp"

#include <iostream>
#include <SDL3/SDL_mouse.h>

#include "Base/CommonManagerVariables.hpp"
#include "Components/LevelGridComponent.hpp"
#include "Components/LevelManagerComponent.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"


using namespace Game;


void TestMousePositionCommand::Execute()
{
    float x{};
    float y{};
    SDL_GetMouseState(&x, &y);

    std::cout << "MousePos X: " << static_cast<int>(x) << ", Y: " << static_cast<int>(y) << '\n';

    bae::Scene* scene      = bae::SceneManager::GetInstance().GetScene(g_LevelManagersSceneName.data());
    auto objects           = scene->GetObjects();
    auto levelGridObjectIt = std::ranges::find_if(objects, [](const auto& object)
    {
        return object->template HasComponent<LevelManagerComponent>();
    });
    if(levelGridObjectIt == objects.end())
    {
        return;
    }

    const auto levelManagerComp = (*levelGridObjectIt)->GetComponent<LevelManagerComponent>();
    if(!levelManagerComp)
    {
        return;
    }

    const auto levelGridComp = levelManagerComp->GetLevelGridComponent();
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
