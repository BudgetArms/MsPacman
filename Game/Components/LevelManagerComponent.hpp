#pragma once
#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include "LevelGridComponent.hpp"
#include "Components/Component.hpp"
#include "Core/Observer.hpp"
#include "Core/SpriteSheet.hpp"

#include "Base/GameMode.hpp"
#include "Base/Level.hpp"
#include "Core/EventListener.hpp"


namespace Game
{
    class LevelManagerComponent : public bae::Component, public bae::Observer, public bae::EventListener
    {
    public:
        LevelManagerComponent(bae::GameObject& owner, GameMode gameMode);
        ~LevelManagerComponent() override;

        void Render() const override;

        void LoadLevelFromFile(int levelNumber, const std::filesystem::path& jsonFile);

        void CreateLevel();
        void ResetLevel();


        [[nodiscard]] LevelGridComponent* GetLevelGridComponent() const;

        [[nodiscard]] int GetCurrentLevelNumber() const;
        [[nodiscard]] LevelJson GetCurrentLevel();

        void Notify(unsigned int eventHash, bae::Subject* subject, const std::any&) override;

        void SetSpriteSheetWorldLocation(const glm::vec2& location) const;
        void SetSpriteSheetWorldRotation(float rotation) const;
        void SetSpriteSheetWorldScale(const glm::vec2& scale) const;

    private:
        void WonLevel();
        void LostLevel();

        void LoadBackground(int levelIndex) const;

        void ClearLevel() const;

        [[nodiscard]] std::optional<LevelJson> GetCurrentLevelJson();

        void CreateGrid();

        void HandleEvent(unsigned int eventHash) override;
        void HandlePlayerDied() const;

        void AddPlayers() const;
        void AddGhosts() const;
        void AddItems() const;


        std::unique_ptr<bae::SpriteSheet> m_BackgroundSpriteSheet{};

        GameMode m_GameMode;


        int m_CurrentLevel{};

        LevelGridComponent* m_LevelGridComponent{};

        std::unordered_map<int, LevelJson> m_LevelJson{};

        const std::filesystem::path m_BackgroundTexturePath{ "Textures/Level/Levels.png" };
        static constexpr int m_LevelRepeatTimes{ 2 };
        static constexpr int m_LevelTextures{ 4 };
    };
}
