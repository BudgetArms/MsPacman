#pragma once

#include "Base/DirectionEnum.hpp"
#include "Components/Component.hpp"
#include "Graphs/AStar.hpp"
#include "Graphs/GridGraph.hpp"


namespace Game
{
    class LevelGridGraph;

    class LevelGridComponent final : public bae::Component
    {
    public:
        LevelGridComponent(bae::GameObject& owner, int columns, int rows, const glm::ivec2& cellSize);
        LevelGridComponent(bae::GameObject& owner, const glm::vec2& gridSize, int columns, int rows);
        ~LevelGridComponent() override;


        void Render() const override;

        // Nodes
        void AddNode(int nodeId) const;
        void AddNode(const glm::vec2& positionNode) const;

        void RemoveNode(int nodeId) const;
        void RemoveNode(const glm::vec2& positionNode) const;

        // Connections
        void AddConnection(int nodeId, Direction direction) const;
        void AddConnection(const glm::vec2& positionNode, Direction direction) const;

        void RemoveConnection(int nodeId, Direction direction) const;
        void RemoveConnection(const glm::vec2& positionNode, Direction direction) const;


        [[nodiscard]] bool GetRenderNodes() const;
        void SetRenderNodes(bool bRenderNodes) const;

        [[nodiscard]] bool GetRenderConnections() const;
        void SetRenderConnections(bool bRenderConnections) const;


        [[nodiscard]] bool IsInGrid(const glm::vec2& position) const;
        [[nodiscard]] bool IsInGrid(const bae::Graphs::GridPosition& gridPosition) const;


        [[nodiscard]] std::vector<glm::vec2> GetShortestPath(int startNodeId, int endNodeId) const;
        [[nodiscard]] std::vector<glm::vec2> GetShortestPath(const glm::vec2& startPos,
                                                             const glm::vec2& endPos) const;

        void UpdateShortestPath();

    private:
        std::unique_ptr<LevelGridGraph> m_LevelGridGraph;
        bae::Graphs::AStar m_AStar;

        //
        std::vector<glm::vec2> m_ShortestPath{};
    };
}

