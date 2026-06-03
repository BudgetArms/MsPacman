#include "TestDamageCommand.hpp"

#include "Components/LifeComponent.hpp"
#include "Core/HelperFunctions.hpp"


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

    std::cout << FUNCTION_NAME << " removing one life" << '\n';
    lifeComponent->RemoveLife();
    std::cout << FUNCTION_NAME << " removed one life, lives left: " << lifeComponent->GetLives() << '\n';
}
