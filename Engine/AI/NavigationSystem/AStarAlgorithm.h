#pragma once

#include "Node.h"
#include <vector>

class AStarAlgorithm
{
private:
    struct Direction
    {
        int x;
        int y;
        float cost;
    };

    
public:
    AStarAlgorithm();
    ~AStarAlgorithm();
    
private:    
    void Clear();

/* Path Finder */    
public:
    std::vector<Position> FindPath(const Position& startPosition, const Position& goalPosition, std::vector<std::vector<int>>& grid);  
    
private:

    Node* CreateNode(const Position& position, Node* parentNode = nullptr);

    std::vector<Position> ConstructPath(Node* destinationNode);

    float CalculateHeuristic(const Position& currentPosition, const Position& goalPosition) const;
    
    Node* FindOpenNode(int x, int y) const;
    
    bool IsValidGrid(const std::vector<std::vector<int>>& grid) const;
    bool IsInRange(int x, int y, const std::vector<std::vector<int>>& grid) const;		
    bool IsDiagonalBlocked(const Position& currentPosition, const Direction& direction,const std::vector<std::vector<int>>& grid) const;
    bool IsInClosedList(int x, int y) const;
    bool IsDestination(const Node* node) const;
    
/* Visualize */    
public:
    void DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Position>& path);

private:
    void ClearVisualization(std::vector<std::vector<int>>& grid) const;
    void DisplayGrid(const std::vector<std::vector<int>>& grid) const;

private:
    std::vector<Node*> allocatedNodes;

    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    Node* startNode;
    Node* goalNode;
};