#include "MsPacmanMoving.hpp"

#include <iostream>

#include "Components/SpriteComponent.hpp"
#include "Components/TextComponent.hpp"

#include "Components/LifeComponent.hpp"
#include "Components/MsPacmanComponent.hpp"
#include "States/Entities/MsPacmanDying.hpp"


using namespace Game::States;


void MsPacmanMoving::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    const auto textComponent = m_GameObject->GetComponent<bae::TextComponent>();
    if(!textComponent)
    {
        return;
    }
    textComponent->SetColor(bae::Utils::Color::Green);
    textComponent->SetText(FUNCTION_NAME);
}

void MsPacmanMoving::OnExit()
{
    std::cout << FUNCTION_NAME << '\n';
}

std::unique_ptr<EntityState> MsPacmanMoving::Update()
{
    return nullptr;
}

void MsPacmanMoving::HandleInput()
{
}
