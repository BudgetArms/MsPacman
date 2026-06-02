#include "Level.hpp"


using namespace Game;


void Game::from_json(const nlohmann::json& json, LevelJson& level)
{
    level =
    {
        .Name = json.at("Name"),

        .NrColumns = json.at("Grid").at("Columns"),
        .NrRows    = json.at("Grid").at("Rows"),

        .Dimensions =
        {
            .Width  = json.at("Grid").at("Dimensions").at("Width"),
            .Height = json.at("Grid").at("Dimensions").at("Height"),
        },

        .PlayerStart =
        {
            .Column = json.at("PlayerStart").at("Column"),
            .Row    = json.at("PlayerStart").at("Row"),
        },

        .AmountOfDotsCollectedForBonusItem = 0,

        .ScoreMap =
        {
            {
                ScoreTypes::PacDot, 10
            },
            {
                ScoreTypes::PowerPellet, 10
            },
            {
                ScoreTypes::GhostOneInSuccession, 10
            },
            {
                ScoreTypes::GhostTwoInSuccession, 10
            },
            {
                ScoreTypes::GhostThreeInSuccession, 10
            },
            {
                ScoreTypes::GhostFourInSuccession, 10
            },
            {
                ScoreTypes::BonusItem, 10
            },
        },
    };

    for(const auto& mirrorAreaPosition : json.at("MirrorAreas"))
    {
        const bae::Graphs::GridPosition gridPosition
        {
            .Column = mirrorAreaPosition.at("Column"),
            .Row    = mirrorAreaPosition.at("Row")
        };

        level.MirrorAreaPositions.insert(gridPosition);
    }


    for(const auto& nodeColumnToRemove : json.at("NodeColumnsToRemove"))
    {
        bae::Graphs::GridPosition gridPosition
        {
            .Column = nodeColumnToRemove,
            .Row    = 0,
        };

        for(int i{}; i < level.NrRows; ++i)
        {
            level.NodesToRemove.insert(gridPosition);
            ++gridPosition.Row;
        }
    }

    for(const auto& nodeRowToRemove : json.at("NodeRowsToRemove"))
    {
        bae::Graphs::GridPosition gridPosition
        {
            .Column = 0,
            .Row    = nodeRowToRemove,
        };

        for(int i{}; i < level.NrColumns; ++i)
        {
            level.NodesToRemove.insert(gridPosition);
            ++gridPosition.Column;
        }
    }

    for(const auto& nodeToRemove : json.at("NodesToRemove"))
    {
        bae::Graphs::GridPosition gridPosition
        {
            .Column = nodeToRemove.at("Column"),
            .Row    = nodeToRemove.at("Row"),
        };

        for(int i{}; i < nodeToRemove.at("Width"); ++i)
        {
            for(int j{}; j < nodeToRemove.at("Height"); ++j)
            {
                level.NodesToRemove.insert(gridPosition);
                ++gridPosition.Row;
            }
            ++gridPosition.Column;
            gridPosition.Row = nodeToRemove.at("Row");
        }
    }


    for(const auto& nodeToAdd : json.at("NodesToAddAfterRemoval"))
    {
        bae::Graphs::GridPosition gridPosition
        {
            .Column = nodeToAdd.at("Column"),
            .Row    = nodeToAdd.at("Row"),
        };

        for(int i{}; i < nodeToAdd.at("Width"); ++i)
        {
            for(int j{}; j < nodeToAdd.at("Height"); ++j)
            {
                level.NodesToAddAfterRemoval.insert(gridPosition);
                ++gridPosition.Row;
            }
            ++gridPosition.Column;
            gridPosition.Row = nodeToAdd.at("Row");
        }
    }
}
