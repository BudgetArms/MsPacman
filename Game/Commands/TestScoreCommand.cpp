#include "TestScoreCommand.hpp"

#include "Components/ScoreComponent.hpp"
#include "Core/HelperFunctions.hpp"


using namespace Game;


TestScoreCommand::TestScoreCommand(bae::GameObject& actor) :
    GameActorCommand(actor)
{
}

void TestScoreCommand::Execute()
{
    if(!GetActor())
    {
        return;
    }

    auto const scoreComponent = GetActor()->GetComponent<ScoreComponent>();
    if(!scoreComponent)
    {
        return;
    }

    std::cout << FUNCTION_NAME << " Adding Score" << '\n';
    scoreComponent->AddScore(100);
    std::cout << FUNCTION_NAME << " Added Score: 100 score gained: " << scoreComponent->GetScore() << '\n';
}
