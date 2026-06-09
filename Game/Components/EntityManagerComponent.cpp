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
        case Events::GameOver:
        case Events::LevelWon:
        case Events::LevelLost:
        case Events::GhostDied:
        case Events::BeginLevel:
        case Events::RestartLevel:
        case Events::ScoreChanged:
        case Events::LivesChanged:
        case Events::InvincibilityChanged:
        case Events::NoEvent:
        case Events::Collision:
            break;
    }
}



