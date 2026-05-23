#include "LevelGridComponent.hpp"

#include "Base/LevelGridGraph.hpp"

using namespace Game;


LevelGridComponent::LevelGridComponent(bae::GameObject& owner, int columns, int rows, const glm::ivec2& cellSize) :
    Component(owner),
    m_LevelGridGraph{ std::make_unique<LevelGridGraph>(owner.GetWorldLocation(), columns, rows, cellSize) }
{
}

LevelGridComponent::LevelGridComponent(bae::GameObject& owner, const glm::vec2& gridSize, const int columns,
                                       const int rows) :
    LevelGridComponent(owner, columns, rows,
                       glm::ivec2(static_cast<float>(gridSize.x) / columns, static_cast<float>(gridSize.y) / rows))
{
}

LevelGridComponent::~LevelGridComponent()
{
}

void LevelGridComponent::Render() const
{
    m_LevelGridGraph->Render();
}

bool LevelGridComponent::GetRenderNodes() const
{
    return m_LevelGridGraph->m_bRenderNodes;
}

void LevelGridComponent::SetRenderNodes(const bool bRenderNodes) const
{
    m_LevelGridGraph->m_bRenderNodes = bRenderNodes;
}

bool LevelGridComponent::IsInGrid(const glm::vec2& position) const
{
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(position);
    return IsInGrid(gridPosition);
}

bool LevelGridComponent::IsInGrid(const bae::Graphs::GridPosition& gridPosition) const
{
    return m_LevelGridGraph->IsWithinBounds(gridPosition);
}
