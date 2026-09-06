#define NOMINMAX

#include "AStarAlgorithm.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <World/World.h>
#include "Actor/Visualizer.h"

AStarAlgorithm::AStarAlgorithm()
	: startNode(nullptr), goalNode(nullptr)
{
}

AStarAlgorithm::~AStarAlgorithm()
{
	Clear();
}

void AStarAlgorithm::Clear()
{
	/* 메모리 해제작업 */
	for (Node* node : allocatedNodes) delete node;

	allocatedNodes.clear();
	openList.clear();
	closedList.clear();

	startNode = nullptr;
	goalNode = nullptr;
}

Node* AStarAlgorithm::CreateNode(const Position& position, Node* parentNode)
{
	Node* node = new Node(position, parentNode);
	allocatedNodes.emplace_back(node);
	return node;
}

std::vector<Position> AStarAlgorithm::FindPath(const Position& startPosition, const Position& goalPosition,	std::vector<std::vector<int>>& grid, UWorld* world)
{
	Clear();

	// if (!IsValidGrid(grid)) return {};
	// if (!IsInRange(startPosition.x, startPosition.y, grid) || !IsInRange(goalPosition.x, goalPosition.y, grid)) return {}; 
	//if (grid[startPosition.y][startPosition.x] == 1 || grid[goalPosition.y][goalPosition.x] == 1) return {};
	
	/* 시작노드·목표노드 설정 */
	startNode = CreateNode(startPosition);
	goalNode = CreateNode(goalPosition);

	/* 최초 비용 초기화 */
	startNode->gCost = 0.0f;
	startNode->hCost = CalculateHeuristic(startPosition, goalPosition);
	startNode->fCost = startNode->gCost + startNode->hCost;
	
	/* openList는 발견했지만, 아직 조사하지 않은 후보노드 목록 */
	openList.emplace_back(startNode);

	/* 방향과 비용 설정 */
	const float straightCost = 1.f;
	const float diagonalCost = 1.41421356f;
	const std::vector<Direction> directions =
	{
		{ 0, -1, straightCost  }, { 0, 1, straightCost },
		{ -1, 0, straightCost  }, { 1, 0, straightCost },
		{ -1, -1, diagonalCost }, {1, -1, diagonalCost },
		{ -1, 1, diagonalCost  }, { 1, 1, diagonalCost }
	};

	/* 목적지를 찾을때까지 탐색 시작 */
	while (!openList.empty())
	{
		/* openList를 전부 검사, fCost가 가장 작은 노드를 currentNode로 설정 */
		Node* currentNode = openList[0];
		for (Node* node : openList)
		{
			if (node->fCost < currentNode->fCost || (node->fCost == currentNode->fCost && node->hCost < currentNode->hCost))  // fCost가 같다면 hCost가 적은쪽이 우선
			{
				currentNode = node;
			}
		}
		
		/* 현재 currentNode는 모든 openList중 가장 fCost가 낮은 노드 */
		
		/* COMPLETE : 목표에 도착했을 경우 최종경로만을 담은 배열반환 */
		if (IsDestination(currentNode)) return ConstructPath(currentNode);

		/* currentNode는 이제 탐색후보목록이 아닌, 이제부터 탐색을 시작하는 노드이므로 closedNode로 이동 */
		auto iterator = std::find(openList.begin(), openList.end(), currentNode);
		if (iterator != openList.end()) openList.erase(iterator);
		closedList.emplace_back(currentNode);

		/* currentNode가 이동할수있는 8방향의 노드를 체크한다 */
		for (const Direction& direction : directions)
		{
			/* 현재위치 + direction의 방향값을 더해 다음위치를 계산 */
			int newX = currentNode->position.x + direction.x;
			int newY = currentNode->position.y + direction.y;

			/* 다음위치가 범위를 벗어나거나 */
			if (!IsInRange(newX, newY, grid)) continue;

			/* 다음위치에 장애물이 있다면 continue */
			if (grid[newY][newX] == 1) continue;

			/* 장애물이 있는데 대각선이동을 시도할경우 블락 */
			if (IsDiagonalBlocked(currentNode->position, direction, grid)) continue;

			/* closeNode는 이동선택지에서 제외 */
			if (IsInClosedList(newX, newY)) continue;

			/* 여태까지 사용한 gCost + 다음이동까지 필요한 cost를 더해 새로운 gCost를 생성 */
			float newGCost = currentNode->gCost + direction.cost;

			/* 만약 다음 탐색하고자하는 노드가 이미 openNode목록에 있다면, 거기까지 가는데 걸리는 비용gCost가 더 싼쪽이 이제 정식 루트가 된다(parent = currentNode)  */
			Node* openNode = FindOpenNode(newX, newY);
			if (openNode != nullptr)
			{
				if (newGCost < openNode->gCost)
				{
					openNode->gCost = newGCost;
					openNode->fCost = openNode->gCost + openNode->hCost;
					openNode->parentNode = currentNode;
				}
				continue;
			}

			/* 다음위치가 openList에 없었다면 새로 노드를 생성한다 */
			Node* neighborNode = CreateNode(Position(newX, newY), currentNode);
			neighborNode->gCost = newGCost;
			neighborNode->hCost = CalculateHeuristic(neighborNode->position, goalNode->position);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			/* 새 노드를 openList에 추가한다 */
			openList.emplace_back(neighborNode);

			/* 시각화 작업 */
			//if (grid[newY][newX] == 0) grid[newY][newX] = 5;
			//DisplayGrid(grid);
			
			/* 월드주고 스폰액터해야함 */
			world->SpawnActor<Visualizer>(Vector3(neighborNode->position.x, 1.f, neighborNode->position.y)); 
			Sleep(50);
		}
	}

	return {};
}

std::vector<Position> AStarAlgorithm::ConstructPath(Node* destinationNode)
{
	/* FindPath 알고리즘이 목적지에 도달했을때, 부모를 따라가며 '최종경로'만 남긴다 */
	std::vector<Position> path;
	Node* currentNode = destinationNode;

	while (currentNode != nullptr)
	{
		path.emplace_back(currentNode->position);
		currentNode = currentNode->parentNode;
	}

	std::reverse(path.begin(), path.end());
	return path;
}

float AStarAlgorithm::CalculateHeuristic(const Position& currentPosition, const Position& goalPosition) const
{
	/* 목표위치까지의 단순 x,y 거리를 구한다 */
	int differenceX = std::abs(currentPosition.x - goalPosition.x);
	int differenceY = std::abs(currentPosition.y - goalPosition.y);

	/* 옥타일 휴리스틱 : 일단 대각선으로 최대한 갈수있는만큼 가는 거리를 구한다 */
	int diagonalDistance = std::min(differenceX, differenceY);
	
	/* 그 다음 남은 거리는 직선이동으로 가야하는 거리를 구한다 */
	int straightDistance = std::max(differenceX, differenceY) - diagonalDistance;

	/* 이동비용 설정 */
	const float diagonalCost = 1.41421356f;
	const float straightCost = 1.0f;

	/* 최종 비용 리턴 */
	return diagonalDistance * diagonalCost + straightDistance * straightCost;
}

bool AStarAlgorithm::IsValidGrid(const std::vector<std::vector<int>>& grid) const
{
	if (grid.empty() || grid[0].empty()) return false;
	{
		
	}

	size_t width = grid[0].size();
	for (const std::vector<int>& row : grid)
	{
		if (row.size() != width)
		{
			return false;
		}
	}

	return true;
}

bool AStarAlgorithm::IsInRange(int x, int y, const std::vector<std::vector<int>>& grid) const
{
	return x >= 0 &&
		x < static_cast<int>(grid[0].size()) &&
		y >= 0 &&
		y < static_cast<int>(grid.size());
}

bool AStarAlgorithm::IsDiagonalBlocked(const Position& currentPosition,	const Direction& direction,	const std::vector<std::vector<int>>& grid) const
{
	if (direction.x == 0 || direction.y == 0) return false;  // 직선이라면 바로 return

	int sideX = currentPosition.x + direction.x;
	int sideY = currentPosition.y + direction.y;

	return grid[currentPosition.y][sideX] == 1 || grid[sideY][currentPosition.x] == 1;  // 대각선 양옆 중 하나라도 장애물이면 이동을 막는다
}

Node* AStarAlgorithm::FindOpenNode(int x, int y) const
{
	/* 파라미터로 들어온 위치와 같은 위치를 가진 노드가 openList에 이미 있는지 체크 */
	for (Node* node : openList)
	{
		if (node->position == Position(x, y)) return node;
	}	
	return nullptr;
}

bool AStarAlgorithm::IsInClosedList(int x, int y) const
{
	/* 파라미터로 들어온 위치와 같은 위치를 가진 노드가 closedList에 이미 있는지 체크 */
	for (Node* node : closedList)
	{ 
		if (node->position == Position(x, y)) return true;
	}
	return false;
}

bool AStarAlgorithm::IsDestination(const Node* node) const
{
	return node != nullptr && goalNode != nullptr && node->position == goalNode->position;
}


void AStarAlgorithm::DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Position>& path)
{

	for (const Position& position : path)
	{
		// 시작점과 목표점은 기존 표시를 유지한다.
		int value = grid[position.y][position.x];
		if (value == 2 || value == 3)
		{
			continue;
		}

		COORD consolePosition =
		{
			static_cast<short>(position.x * 2), static_cast<short>(position.y)
		};

		Sleep(50);
	}
}