#pragma once

#include "Commands/GameActorCommand.hpp"

#include "Components/LevelManagerComponent.hpp"


namespace Game
{
    class LevelManagerComponent;

    class SkipLevelCommand final : public bae::GameActorCommand
    {
    public:
        explicit SkipLevelCommand(bae::GameObject& owner);

        void Execute() override;

    private:
        LevelManagerComponent* m_LevelManagerComponent{};
    };
}

