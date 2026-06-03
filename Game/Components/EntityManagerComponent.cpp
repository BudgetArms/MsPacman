#include "EntityManagerComponent.hpp"

#include "Base/Events.hpp"

using namespace Game;


EntityManagerComponent::EntityManagerComponent(bae::GameObject& owner) :
    Component(owner)
{
}

void EntityManagerComponent::AddPlayerToScene(std::shared_ptr<bae::GameObject>)
{
}

void EntityManagerComponent::AddToScene(std::shared_ptr<bae::GameObject>)
{
}

void EntityManagerComponent::HandleEvent(const unsigned eventHash)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
            break;
        case Events::GameOver:
            break;
        case Events::LevelWon:
            break;
        case Events::LevelLost:
            break;
        case Events::GhostDied:
            break;
        case Events::BeginLevel:
            break;
        case Events::RestartLevel:
            break;
        case Events::ScoreChanged:
            break;
        case Events::LivesChanged:
            break;
        case Events::InvincibilityChanged:
            break;
        case Events::NoEvent:
            break;
    }
}



