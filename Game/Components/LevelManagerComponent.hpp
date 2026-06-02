#pragma once
#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

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


        [[nodiscard]] int GetCurrentLevel() const;

        void Notify(unsigned int eventHash, bae::Subject* subject) override;

        void SetSpriteSheetWorldLocation(const glm::vec2& location) const;
        void SetSpriteSheetWorldRotation(float rotation) const;
        void SetSpriteSheetWorldScale(const glm::vec2& scale) const;

    private:
        void WonLevel();
        void LostLevel();

        void LoadBackground(int levelIndex) const;

        void ClearLevel() const;

        [[nodiscard]] std::optional<LevelJson> GetCurrentLevelJson();

        void CreateBackground();
        void CreateGrid();

        void HandleEvent(unsigned int eventHash) override;
        void HandlePlayerDied() const;


        std::unique_ptr<bae::SpriteSheet> m_BackgroundSpriteSheet{};

        GameMode m_GameMode;


        int m_CurrentLevel{};


        std::unordered_map<int, LevelJson> m_LevelJson{};

        const std::filesystem::path m_BackgroundTexturePath{ "Textures/Level/Levels.png" };
        const std::string m_LevelSceneName{ "LevelScene" };
        static constexpr int m_LevelRepeatTimes{ 2 };
        static constexpr int m_LevelTextures{ 4 };
    };
}
