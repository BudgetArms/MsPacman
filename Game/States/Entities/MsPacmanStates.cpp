#include "MsPacmanStates.hpp"

#include "Core/GameObject.hpp"


using namespace Game::States;


MsPacmanState::MsPacmanState(bae::GameObject& gameObject) :
    m_GameObject{ &gameObject }
{
}

void MsPacmanState::OnResume()
{
    m_bIsInputPaused = false;
}

void MsPacmanState::OnSuspend()
{
    m_bIsInputPaused = true;
}

bool MsPacmanState::IsInputPaused() const
{
    return m_bIsInputPaused;
}


