#pragma once

#include <set>
#include <nlohmann/json.hpp>

#include "Core/Renderer.hpp"
#include "Graphs/GridGraph.hpp"


namespace Game
{
    enum class ScoreTypes
    {
        PacDot,
        PowerPellet,
        GhostOneInSuccession,
        GhostTwoInSuccession,
        GhostThreeInSuccession,
        GhostFourInSuccession,
        BonusItem,
    };


    struct LevelJson
    {
        std::string Name{};
        int NrColumns{};
        int NrRows{};
        bae::WindowSize Dimensions{};

        bae::Graphs::GridPosition PlayerStart{};
        std::set<bae::Graphs::GridPosition> MirrorAreaPositions{};

        int AmountOfDotsCollectedForBonusItem{};

        std::unordered_map<ScoreTypes, int> ScoreMap{};

        std::set<bae::Graphs::GridPosition> NodesToRemove{};
        std::set<bae::Graphs::GridPosition> NodesToAddAfterRemoval{};
    };


    void from_json(const nlohmann::json& json, LevelJson& level);
}
