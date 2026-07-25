#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>

#include "Components/Component.hpp"
#include "Core/SpriteSheet.hpp"

#include "Base/GameMode.hpp"
#include "Base/Level.hpp"
#include "Components/LevelGridComponent.hpp"
#include "Core/EventListener.hpp"


namespace Game
{
    class LevelManager : public bae::Singleton<LevelManager>, public bae::EventListener
    {
    public:
        explicit LevelManager();
        ~LevelManager() override;

        void RenderBackground() const;

        void LoadLevelFromFile(int levelNumber, const std::filesystem::path& jsonFile);

        void CreateLevel();
        void ResetLevel();

        void SkipLevel();


        [[nodiscard]] LevelGridComponent* GetLevelGridComponent() const;

        [[nodiscard]] int GetCurrentLevelNumber() const;
        [[nodiscard]] LevelJson GetCurrentLevel();

        void SetGameMode(GameMode gameMode);

        void SetSpriteSheetWorldLocation(const glm::vec2& location) const;
        void SetSpriteSheetWorldRotation(float rotation) const;
        void SetSpriteSheetWorldScale(const glm::vec2& scale) const;

    private:
        void WonLevel();
        void LostLevel();

        void LoadBackground() const;

        void ClearLevel() const;

        [[nodiscard]] std::optional<LevelJson> GetCurrentLevelJson();

        void CreateGrid();

        void HandleEvent(unsigned int eventHash) override;
        void HandlePlayerDied() const;

        void AddPlayers() const;
        void AddGhosts() const;
        void AddItems() const;


        static std::shared_ptr<bae::GameObject> GetGhostBase(const std::string& gameObjectName,
                                                             const glm::vec2& spawnPosition);

        std::shared_ptr<bae::GameObject> GetMsPacmanBase(const std::string& gameObjectName,
                                                         const glm::vec2& spawnPosition) const;

        void SpawnBlinky() const;

        void SpawnMsPacman() const;
        void SpawnMrPacman() const;

        static void SpawnFruit(const glm::vec2& position);
        static void SpawnDot(const glm::vec2& position);
        static void SpawnPellet(const glm::vec2& position);

        static void AddControls(bae::GameObject& gameObject, bool firstPlayer);


        std::unique_ptr<bae::SpriteSheet> m_BackgroundSpriteSheet{};

        GameMode m_GameMode{ GameMode::Singleplayer };


        int m_CurrentLevel{ 0 };

        LevelGridComponent* m_LevelGridComponent{};

        std::unordered_map<int, LevelJson> m_LevelJson{};

        // static constexpr glm::vec2 test{ 20, 20 };

        const std::filesystem::path m_BackgroundTexturePath{ "Textures/Level/Levels.png" };
        static constexpr int m_LevelRepeatTimes{ 1 };
        static constexpr int m_LevelTextures{ 4 };
    };
}
