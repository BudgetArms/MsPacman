#include "GhostStates.hpp"

#include "Core/HelperFunctions.hpp"

#include "Components/GridMovementComponent.hpp"


using namespace Game;


States::GhostStates::GhostStates(bae::GameObject& gameObject) :
    m_GameObject{ &gameObject }
{
    m_GridMovementComponent = gameObject.GetComponent<GridMovementComponent>();
    if(!m_GridMovementComponent)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Get GridMovementComponent"));
    }
}

void States::GhostStates::OnResume()
{
    m_bIsPaused = false;
}

void States::GhostStates::OnSuspend()
{
    m_bIsPaused = true;
}

bool States::GhostStates::IsPaused() const
{
    return m_bIsPaused;
}
