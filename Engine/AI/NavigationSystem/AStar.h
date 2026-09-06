#pragma once

#include "Node.h"
#include <vector>

class AStarAlgorithm
{
private:
    // 이동 방향과 해당 방향의 이동 비용.
    struct Direction
    {
        int x;
        int y;
        float cost;
    };

public:
    AStarAlgorithm();
    ~AStarAlgorithm();

    // 시작 위치에서 목표 위치까지 경로를 탐색한다.
    std::vector<Position> FindPath(
        const Position& startPosition,
        const Position& goalPosition,
        std::vector<std::vector<int>>& grid);

    // 탐색한 최종 경로를 그리드에 출력한다.
    void DisplayGridWithPath(
        std::vector<std::vector<int>>& grid,
        const std::vector<Position>& path);

private:
    // 이전 탐색에서 생성한 노드와 목록을 정리한다.
    void Clear();

    // AStar가 소유할 탐색 노드를 생성한다.
    Node* CreateNode(
        const Position& position,
        Node* parentNode = nullptr);

    // 부모 노드를 역추적하여 경로를 구성한다.
    std::vector<Position> ConstructPath(Node* destinationNode);

    // 목표 위치까지의 예상 비용을 계산한다.
    float CalculateHeuristic(
        const Position& currentPosition, const Position& goalPosition) const;
		
    // 대각선 이동이 장애물 모서리를 통과하는지 확인한다.
    bool IsDiagonalBlocked(
        const Position& currentPosition,
        const Direction& direction,
        const std::vector<std::vector<int>>& grid) const;

    // Open 목록과 Closed 목록을 검사한다.
    Node* FindOpenNode(int x, int y) const;
    bool IsInClosedList(int x, int y) const;
    bool IsDestination(const Node* node) const;

    // 이전 탐색 표시를 지우고 그리드를 출력한다.
    void ClearVisualization(std::vector<std::vector<int>>& grid) const;
    void DisplayGrid(const std::vector<std::vector<int>>& grid) const;

private:
    // 동적 할당한 모든 노드를 소유한다.
    std::vector<Node*> allocatedNodes;

    // 탐색할 노드와 탐색을 마친 노드 목록.
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    Node* startNode;
    Node* goalNode;
};