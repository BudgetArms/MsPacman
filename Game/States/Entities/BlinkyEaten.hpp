#pragma once

#include <SDL3/SDL.h>

#include "States/Entities/GhostStates.hpp"

#include "Base/GhostMovement.hpp"


namespace Game::States
{
    class BlinkyEaten : public GhostStates
    {
    public:
        explicit BlinkyEaten(bae::GameObject& gameObject, const glm::vec2& spawnPosition);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

    private:
        GhostMovement m_Movement;

        const float m_TimeDeathCooldownTime{ 6.f };
        float m_TimeDeathElapsed{};


        static constexpr std::string_view m_GhostEatenSpritePath = "Textures/Characters/InvisibleGhost.png";
        static constexpr SDL_FRect m_GhostEatenSrcRect           = { 0, 0, 32, 64 };
        static constexpr int m_GhostEatenNrSprites{ 8 };
        static constexpr int m_NrColumnsSprite{ 2 };
    };
}
