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

LevelGridComponent::~LevelGridComponent() = default;


void LevelGridComponent::Render() const
{
    m_LevelGridGraph->Render();
}

void LevelGridComponent::AddConnection(const glm::vec2& positionNode, const Direction direction) const
{
    const int nodeId                             = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(positionNode);

    std::vector<glm::vec2> directions{};
    switch(direction)
    {
        case Direction::Left:
            directions.push_back({ -1, 0 });
            break;
        case Direction::Right:
            directions.push_back({ 1, 0 });
            break;
        case Direction::Up:
            directions.push_back({ 0, -1 });
            break;
        case Direction::Down:
            directions.push_back({ 0, 1 });
            break;
    }

    m_LevelGridGraph->AddConnectionsInDirections(nodeId, gridPosition, directions);
}

void LevelGridComponent::RemoveConnection(const glm::vec2& positionNode, const Direction direction) const
{
    const int nodeId                             = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(positionNode);

    std::vector<glm::vec2> directions{};
    switch(direction)
    {
        case Direction::Left:
            directions.push_back({ -1, 0 });
            break;
        case Direction::Right:
            directions.push_back({ 1, 0 });
            break;
        case Direction::Up:
            directions.push_back({ 0, -1 });
            break;
        case Direction::Down:
            directions.push_back({ 0, 1 });
            break;
    }

    m_LevelGridGraph->RemoveConnectionsInDirections(nodeId, gridPosition, directions);
}

bool LevelGridComponent::GetRenderNodes() const
{
    return m_LevelGridGraph->m_bRenderNodes;
}

void LevelGridComponent::SetRenderNodes(const bool bRenderNodes) const
{
    m_LevelGridGraph->m_bRenderNodes = bRenderNodes;
}


bool LevelGridComponent::GetRenderConnections() const
{
    return m_LevelGridGraph->m_bRenderConnections;
}

void LevelGridComponent::SetRenderConnections(const bool bRenderConnections) const
{
    m_LevelGridGraph->m_bRenderConnections = bRenderConnections;
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

