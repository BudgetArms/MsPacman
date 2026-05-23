#include "LevelGridGraph.hpp"

#include <iostream>

#include "LevelGraphNode.hpp"
#include "Core/HelperFunctions.hpp"


Game::LevelGridGraph::LevelGridGraph(const glm::vec2& position, const int columns, const int rows,
                                     const glm::ivec2& cellSize) :
    GridGraph(position, columns, rows, cellSize, true, false)
{
}

void Game::LevelGridGraph::SetNodeTerrainType(const int nodeId, const LevelGridType type) const
{
    LevelGraphNode* const pNode = dynamic_cast<LevelGraphNode*>(GetNode(nodeId));
    if(!pNode)
    {
        std::cout << FUNCTION_NAME << " Failed! Node is not valid, NodeId:" << nodeId << '\n';
        return;
    }

    pNode->m_GridType = type;
}

void Game::LevelGridGraph::SetNodeTerrainType(const glm::vec2& position, const LevelGridType type) const
{
    const int nodeId = GetNodeId(GetGridPosition(position));
    return SetNodeTerrainType(nodeId, type);
}

