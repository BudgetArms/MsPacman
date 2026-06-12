#pragma once

#include <SDL3/SDL.h>

#include "States/Entities/GhostStates.hpp"

#include "Base/GhostMovement.hpp"


namespace Game::States
{
    class BlinkyChasing : public GhostStates
    {
    public:
        explicit BlinkyChasing(bae::GameObject& gameObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

    private:
        void UpdateTargetPosition();
        static std::vector<bae::GameObject*> GetPlayersGameObjects();

        GhostMovement m_Movement;

        static constexpr std::string_view m_BlinkySpritePath = "Textures/Characters/Blinky.png";
        static constexpr SDL_FRect m_BlinkySrcRect           = { 0, 0, 32, 64 };
        static constexpr int m_BlinkyNrSprites{ 8 };

        static constexpr int m_BlinkySpriteIndexOffset{ 1 };
        static constexpr int m_NrColumnsSprite{ 2 };
    };
}
