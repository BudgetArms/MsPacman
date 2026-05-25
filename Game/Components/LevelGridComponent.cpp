#include "LevelGridComponent.hpp"

#include "Base/LevelGridGraph.hpp"
#include "Core/HelperFunctions.hpp"


using namespace Game;


LevelGridComponent::LevelGridComponent(bae::GameObject& owner, int columns, int rows, const glm::ivec2& cellSize) :
    Component(owner),
    m_LevelGridGraph{ std::make_unique<LevelGridGraph>(owner.GetWorldLocation(), columns, rows, cellSize) },
    m_AStar{ m_LevelGridGraph.get(), bae::Graphs::HeuristicFunctions::Chebyshev }
{
}

LevelGridComponent::LevelGridComponent(bae::GameObject& owner, const glm::vec2& gridSize, const int columns,
                                       const int rows) :
    LevelGridComponent(owner, columns, rows,
                       glm::ivec2(static_cast<float>(gridSize.x) / columns, static_cast<float>(gridSize.y) / rows))
{
    UpdateShortestPath();
}

LevelGridComponent::~LevelGridComponent() = default;


void LevelGridComponent::Render() const
{
    m_LevelGridGraph->Render();

    // Render AStar Path
    for(const glm::vec2& pos : m_ShortestPath)
    {
        bae::Utils::DrawCircle(pos, 10, bae::Utils::Color::Orange);
    }
}


void LevelGridComponent::AddNode(const int nodeId) const
{
    // Gets existing (grid locked) node position
    const glm::vec2 positionNode                 = m_LevelGridGraph->GetNodePos(nodeId);
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(nodeId);

    if(m_LevelGridGraph->IsWithinBounds(gridPosition))
    {
        m_LevelGridGraph->AddNode(std::make_unique<bae::Graphs::Node>(positionNode));
    }
}

void LevelGridComponent::AddNode(const glm::vec2& positionNode) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    AddNode(nodeId);
}

void LevelGridComponent::RemoveNode(const int nodeId) const
{
    m_LevelGridGraph->RemoveNode(nodeId);
}

void LevelGridComponent::RemoveNode(const glm::vec2& positionNode) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    RemoveNode(nodeId);
}

void LevelGridComponent::AddConnection(const int nodeId, const Direction direction) const
{
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(nodeId);

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

void LevelGridComponent::AddConnection(const glm::vec2& positionNode, const Direction direction) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    AddConnection(nodeId, direction);
}

void LevelGridComponent::RemoveConnection(const int nodeId, const Direction direction) const
{
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(nodeId);

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

void LevelGridComponent::RemoveConnection(const glm::vec2& positionNode, const Direction direction) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    RemoveConnection(nodeId, direction);
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


std::vector<glm::vec2> LevelGridComponent::GetShortestPath(const int startNodeId, const int endNodeId) const
{
    bae::Graphs::Node* startNode     = m_LevelGridGraph->GetNode(m_LevelGridGraph->GetGridPosition(startNodeId));
    const bae::Graphs::Node* endNode = m_LevelGridGraph->GetNode(m_LevelGridGraph->GetGridPosition(endNodeId));

    if(!startNode || !startNode->IsValid())
    {
        return std::vector<glm::vec2>{};
    }
    if(!endNode || !endNode->IsValid())
    {
        return std::vector<glm::vec2>{};
    }

    std::vector<bae::Graphs::Node*> pathNodes = m_AStar.FindPath(startNode, endNode);
    return bae::Graphs::ConvertNodesToPositions(pathNodes);
}


std::vector<glm::vec2> LevelGridComponent::GetShortestPath(const glm::vec2& startPos, const glm::vec2& endPos) const
{
    const int startNodeId = m_LevelGridGraph->GetNodeIdAtPosition(startPos);
    const int endNodeId   = m_LevelGridGraph->GetNodeIdAtPosition(endPos);

    return GetShortestPath(startNodeId, endNodeId);
}

void LevelGridComponent::UpdateShortestPath()
{
    m_ShortestPath = GetShortestPath(0, 99);
}

