#include "MsPacmanMoving.hpp"

#include <iostream>

#include "MsPacmanDying.hpp"
#include "Components/TextComponent.h"
#include "Singletons/GameTime.h"

#include "../Game/Components/MsPacmanComponent.hpp"


using namespace Game::States;


void MsPacmanMoving::OnEnter()
{
    std::cout << FUNCTION_NAME << '\n';

    auto textComponent = m_GameObject->GetComponent<bae::TextComponent>();
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
    if(m_AccumulatedTime == 0.0f)
    {
        std::cout << FUNCTION_NAME << '\n';
    }

    m_AccumulatedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime >= m_TimeSwitch)
    {
        if(m_GameObject->HasComponent<MsPacmanComponent>())
        {
            return std::make_unique<MsPacmanDying>(*m_GameObject);
        }
    }

    return nullptr;
}

void MsPacmanMoving::HandleInput()
{
}
