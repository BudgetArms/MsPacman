#pragma once

#include <memory>


namespace Game ::States
{
    class MenuState
    {
    public:
        MenuState()          = default;
        virtual ~MenuState() = default;

        virtual void OnEnter() =0;
        virtual void OnExit() =0;

        virtual void OnResume() = 0;
        virtual void OnSuspend() = 0;

        virtual std::unique_ptr<MenuState> Update() = 0;
    };
}

