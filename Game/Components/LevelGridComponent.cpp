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
                       glm::ivec2(static_cast<float>(gridSize.x) / static_cast<float>(columns),
                                  static_cast<float>(gridSize.y) / static_cast<float>(rows)))
{
}

LevelGridComponent::~LevelGridComponent() = default;


void LevelGridComponent::Render() const
{
    m_LevelGridGraph->Render();
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

void LevelGridComponent::AddNode(const bae::Graphs::GridPosition& position) const
{
    if(!m_LevelGridGraph->IsWithinBounds(position))
    {
        std::cout << FUNCTION_NAME << " Failed! GridPosition not in Grid!" << '\n';
        return;
    }

    const int nodeId = m_LevelGridGraph->GetNodeId(position);
    AddNode(nodeId);
}

void LevelGridComponent::AddNodeAtPosition(const glm::vec2& positionNode) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    AddNode(nodeId);
}

void LevelGridComponent::RemoveNode(const int nodeId) const
{
    m_LevelGridGraph->RemoveNode(nodeId);
}

void LevelGridComponent::RemoveNode(const bae::Graphs::GridPosition& position) const
{
    if(!m_LevelGridGraph->IsWithinBounds(position))
    {
        std::cout << FUNCTION_NAME << " Failed! GridPosition not in Grid!" << '\n';
        return;
    }

    const int nodeId = m_LevelGridGraph->GetNodeId(position);
    RemoveNode(nodeId);
}

void LevelGridComponent::RemoveNodeAtPosition(const glm::vec2& positionNode) const
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
            directions.emplace_back(-1, 0);
            break;
        case Direction::Right:
            directions.emplace_back(1, 0);
            break;
        case Direction::Up:
            directions.emplace_back(0, -1);
            break;
        case Direction::Down:
            directions.emplace_back(0, 1);
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
            directions.emplace_back(-1, 0);
            break;
        case Direction::Right:
            directions.emplace_back(1, 0);
            break;
        case Direction::Up:
            directions.emplace_back(0, -1);
            break;
        case Direction::Down:
            directions.emplace_back(0, 1);
            break;
    }

    m_LevelGridGraph->RemoveConnectionsInDirections(nodeId, gridPosition, directions);
}

void LevelGridComponent::RemoveConnection(const glm::vec2& positionNode, const Direction direction) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    RemoveConnection(nodeId, direction);
}

void LevelGridComponent::AddConnectionsToNeighbors(const bae::Graphs::GridPosition position) const
{
    m_LevelGridGraph->AddConnectionsToAdjacentCells(position);
}

bool LevelGridComponent::GetRenderCells() const
{
    return m_LevelGridGraph->m_bRenderCells;
}

void LevelGridComponent::SetRenderCells(const bool bRenderCells) const
{
    m_LevelGridGraph->m_bRenderCells = bRenderCells;
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

bae::Graphs::GridPosition LevelGridComponent::GetGridPosition(const glm::vec2& position) const
{
    return m_LevelGridGraph->GetGridPosition(position);
}

glm::vec2 LevelGridComponent::GetPosition(const bae::Graphs::GridPosition gridPosition) const
{
    return m_LevelGridGraph->GetPosition(gridPosition);
}

bool LevelGridComponent::IsValidGridPosition(const bae::Graphs::GridPosition gridPosition) const
{
    if(!m_LevelGridGraph->IsWithinBounds(gridPosition))
    {
        return false;
    }
    const bae::Graphs::Node* node = m_LevelGridGraph->GetNode(gridPosition);
    if(!node || !node->IsValid())
    {
        return false;
    }

    return m_LevelGridGraph->GetNodeId(gridPosition) != bae::Graphs::InvalidNodeID;
}

bae::Graphs::GridPosition LevelGridComponent::GetClosestValidNodePosition(const glm::vec2& position) const
{
    return m_LevelGridGraph->GetClosestValidNodePositionAtPosition(position);
}

int LevelGridComponent::GetColumns() const
{
    return m_LevelGridGraph->GetColumns();
}

int LevelGridComponent::GetRows() const
{
    return m_LevelGridGraph->GetRows();
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

bool LevelGridComponent::DoesConnectionExistInDirection(const bae::Graphs::GridPosition gridPosition,
                                                        const Direction direction) const
{
    if(!IsInGrid(gridPosition))
    {
        return false;
    }

    bae::Graphs::GridPosition newGridPosition = gridPosition;
    switch(direction)
    {
        case Direction::Left:
            --newGridPosition.Column;
            break;
        case Direction::Right:
            ++newGridPosition.Column;
            break;
        case Direction::Up:
            --newGridPosition.Row;
            break;
        case Direction::Down:
            ++newGridPosition.Row;
            break;
    }

    if(!IsInGrid(newGridPosition))
    {
        return false;
    }

    const int nodeId    = m_LevelGridGraph->GetNodeId(gridPosition);
    const int newNodeId = m_LevelGridGraph->GetNodeId(newGridPosition);

    const bae::Graphs::Connection* const connection = m_LevelGridGraph->FindConnection(nodeId, newNodeId);
    if(!connection || !connection->IsValid())
    {
        return false;
    }

    return true;
}

std::vector<glm::vec2> LevelGridComponent::GetALlConnections() const
{
    auto nodes = m_LevelGridGraph->GetActiveNodes();

    return bae::Graphs::ConvertNodesToPositions(nodes);
}


std::vector<glm::vec2> LevelGridComponent::GetShortestPath(const int startNodeId, const int endNodeId) const
{
    const bae::Graphs::GridPosition startPos =
            m_LevelGridGraph->GetClosestValidNodePosition(m_LevelGridGraph->GetGridPosition(startNodeId));
    const bae::Graphs::GridPosition endPos =
            m_LevelGridGraph->GetClosestValidNodePosition(m_LevelGridGraph->GetGridPosition(endNodeId));

    const std::vector<bae::Graphs::Node*> pathNodes = m_AStar.FindPath(m_LevelGridGraph->GetNode(startPos),
                                                                       m_LevelGridGraph->GetNode(endPos));

    return bae::Graphs::ConvertNodesToPositions(pathNodes);
}


std::vector<glm::vec2> LevelGridComponent::GetShortestPath(const glm::vec2& startPos, const glm::vec2& endPos) const
{
    const int startNodeId = m_LevelGridGraph->GetNodeIdAtPosition(startPos);
    const int endNodeId   = m_LevelGridGraph->GetNodeIdAtPosition(endPos);

    return GetShortestPath(startNodeId, endNodeId);
}

