#pragma once

#include "Graphs/GraphNode.hpp"

#include "Base/LevelGridType.hpp"


namespace Game
{
    class LevelGraphNode : public bae::Graphs::GraphNode
    {
    public:
        LevelGraphNode(const glm::vec2& position);

        LevelGridType m_GridType{ LevelGridType::Nothing };
    };
}
