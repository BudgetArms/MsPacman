#include "TestMousePositionCommand.hpp"

#include <iostream>
#include <SDL3/SDL_mouse.h>


using namespace Game;


void TestMousePositionCommand::Execute()
{
    float x{};
    float y{};
    SDL_GetMouseState(&x, &y);

    std::cout << "MousePos X: " << static_cast<int>(x) << ", Y: " << static_cast<int>(y) << '\n';
}
