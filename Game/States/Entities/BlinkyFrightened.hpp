#pragma once

#include <SDL3/SDL.h>

#include "Core/EventListener.hpp"

#include "Base/GhostMovement.hpp"
#include "States/Entities/GhostStates.hpp"


namespace Game::States
{
    class BlinkyFrightened : public GhostStates, public bae::EventListener
    {
    public:
        explicit BlinkyFrightened(bae::GameObject& gameObject, const glm::vec2& frightenedPosition);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

    private:
        void HandleEvent(unsigned eventHash) override;


        GhostMovement m_Movement;

        static constexpr std::string_view m_SpritePath = "Textures/Characters/ScaredGhost.png";
        static constexpr SDL_FRect m_SrcRect           = { 0, 0, 32, 32 };
        static constexpr int m_NrSprites{ 4 };
        static constexpr int m_NrColumnsSprite{ 2 };

        bool m_bIsFrightened{ true };
    };
}

