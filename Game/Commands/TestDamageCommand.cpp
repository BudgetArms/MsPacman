#include "TestDamageCommand.hpp"

#include "Components/LifeComponent.hpp"
#include "Components/MsPacmanComponent.hpp"


using namespace Game;


TestDamageCommand::TestDamageCommand(bae::GameObject& actor) :
    GameActorCommand(actor)
{
}

void TestDamageCommand::Execute()
{
    if(!GetActor())
    {
        return;
    }

    auto const lifeComponent = GetActor()->GetComponent<LifeComponent>();
    if(!lifeComponent)
    {
        return;
    }

    std::cout << "TestDamageCommand: removing one life" << '\n';
    lifeComponent->RemoveLife();
    std::cout << "TestDamageCommand: removed one life, lives left: " << lifeComponent->GetLives() << '\n';
}
