#pragma once

#include <memory>


namespace Game::States
{
    class EntityState
    {
    public:
        EntityState()          = default;
        virtual ~EntityState() = default;

        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;

        virtual void OnResume() = 0;
        virtual void OnSuspend() = 0;

        virtual std::unique_ptr<EntityState> Update() = 0;
    };
}
