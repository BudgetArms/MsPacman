#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <memory>
#include <cassert>

#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <chrono>


#pragma region BudgetArmsEngine Includes


#include <glm.hpp>
#include <SDL.h>
#include <imgui.h>
#include <imgui_plot.h>

#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif


#include "Core/BudgetEngine.h"
#include "Core/GameObject.h"
#include "Core/Renderer.h"


#ifdef STEAMWORKS_ENABLED

#include "Core/Achievement.h"
#include "Managers/SteamManager.h"
#include "Managers/AchievementManager.h"

#endif


#pragma endregion


namespace fs = std::filesystem;
using namespace bae;

void Start();


int main(int, char* [])
{
    //thx myself for dying to find this
#ifdef WIN32
    if (AllocConsole())
    {
        FILE* pEmpty;
        freopen_s(&pEmpty, "CONOUT$", "w", stdout);
        freopen_s(&pEmpty, "CONOUT$", "w", stderr);
    }
#endif


    bae::Utils::Window window{ "Ms Pacman", "./Resources/", 960, 612, false };


#if __EMSCRIPTEN__
    window.resourceFolder = "";
#else

    if (!fs::exists(window.resourceFolder))
        window.resourceFolder = "../Resources/";

    if (!fs::exists(window.resourceFolder))
    {
        std::cout << "Resouces Folder Not Found" << '\n';
        assert("Resouces Folder Not Found");
        exit(-1);
    }

#ifdef STEAMWORKS_ENABLED
    SteamManager::GetInstance().Initialize();

#endif

#endif


    BudgetEngine engine(window);
    engine.Run(Start);


#ifdef STEAMWORKS_ENABLED
    SteamManager::GetInstance().Shutdown();
#endif


    std::cout << "\n\n";


    return 0;
}


void Start()
{


}


