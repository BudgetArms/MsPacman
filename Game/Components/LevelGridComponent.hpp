#pragma once

#include "Components/Component.hpp"
#include "Graphs/GridGraph.hpp"


namespace Game
{
    class LevelGridGraph;

    class LevelGridComponent final : public bae::Component
    {
    public:
        LevelGridComponent(bae::GameObject& owner, int columns, int rows, const glm::ivec2& cellSize);
        LevelGridComponent(bae::GameObject& owner, const glm::vec2& gridSize, int columns, int rows);
        virtual ~LevelGridComponent();

        void Render() const override;

        [[nodiscard]] bool GetRenderNodes() const;
        void SetRenderNodes(bool bRenderNodes) const;

        [[nodiscard]] bool IsInGrid(const glm::vec2& position) const;
        [[nodiscard]] bool IsInGrid(const bae::Graphs::GridPosition& gridPosition) const;

    private:
        std::unique_ptr<LevelGridGraph> m_LevelGridGraph;
    };
}
