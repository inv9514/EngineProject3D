#define NOMINMAX

#include "AStar.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <Windows.h>

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
	for (Node* node : allocatedNodes)
	{
		delete node;
	}

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

std::vector<Position> AStarAlgorithm::FindPath(const Position& startPosition,const Position& goalPosition,std::vector<std::vector<int>>& grid)
{
	Clear();	
	ClearVisualization(grid);

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

		/* COMPLETE : 목표에 도착했을 경우 함수 탈출 */
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

			/* 벽이라면 continue */
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

			/* 만약 기존 openList에 없던 노드라면 새로 노드를 생성한다 */
			Node* neighborNode = CreateNode(Position(newX, newY), currentNode);
			neighborNode->gCost = newGCost;
			neighborNode->hCost = CalculateHeuristic(neighborNode->position, goalNode->position);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			/* 새 노드를 openList에 추가한다 */
			openList.emplace_back(neighborNode);

			/* 시각화 작업 */
			if (grid[newY][newX] == 0) grid[newY][newX] = 5;
			DisplayGrid(grid);
			Sleep(50);
		}
	}

	return {};
}

std::vector<Position> AStarAlgorithm::ConstructPath(Node* destinationNode)
{
	// 목표 노드부터 부모 노드를 따라 역추적한다.
	std::vector<Position> path;
	Node* currentNode = destinationNode;

	while (currentNode != nullptr)
	{
		path.emplace_back(currentNode->position);
		currentNode = currentNode->parentNode;
	}

	// 시작 위치부터 이동하도록 순서를 뒤집는다.
	std::reverse(path.begin(), path.end());
	return path;
}

float AStarAlgorithm::CalculateHeuristic(
	const Position& currentPosition,
	const Position& goalPosition) const
{
	// 옥타일 거리(Octaile Distance) 계산 방식을 사용.
	// 8방향으로 이동을 허용할 때 사용 가능한 계산 방식 중 하나.
	// 1.현재 위치에서 목표 위치까지의 차이를 가로/세로 이동 간격으로 계산한 뒤
	// 2.대각선으로 이동이 가능한 거리는 최대한 대각선 이동 거리로 좁히고
	// 3.직선으로만 이동 가능한 거리만 계산
	// 4.hCost는 아래의 계산 공식으로 계산.
	//   (대각선 이동 횟수 x 대각선 이동 비용(1.414)) + (직선 이동 횟수 x 직선 이동 비용 (1))

	// 현재 위치와 목표 위치의 좌표 차이를 계산한다.
	int differenceX = std::abs(currentPosition.x - goalPosition.x);
	int differenceY = std::abs(currentPosition.y - goalPosition.y);

	// 대각선 거리와 남은 직선 거리를 분리한다.
	// 대각선으로 이동이 가능한 거리 확인을 위해 가로/세로 중 짧은 거리 구하기.
	int diagonalDistance = std::min(differenceX, differenceY);
	
	// 직선으로만 이동 가능한 거리를 구하기 위해 가로/세로 중 긴 거리에서 대각선 이동거리 제외.
	int straightDistance = std::max(differenceX, differenceY) - diagonalDistance;

	// 대각선 이동 비용.
	const float diagonalCost = 1.41421356f;
	// 직선 이동 비용.
	const float straightCost = 1.0f;

	// (대각선 이동 횟수 x 대각선 이동 비용(1.414)) + (직선 이동 횟수 x 직선 이동 비용(1))
	return diagonalDistance * diagonalCost + straightDistance * straightCost;
}

bool AStarAlgorithm::IsDiagonalBlocked(const Position& currentPosition, const Direction& direction, const std::vector<std::vector<int>>& grid) const
{
	// 대각선 이동이 아니면 모서리 검사가 필요하지 않다.
	if (direction.x == 0 || direction.y == 0)
	{
		return false;
	}

	int sideX = currentPosition.x + direction.x;
	int sideY = currentPosition.y + direction.y;

	// 대각선 양옆 중 하나라도 장애물이면 이동을 막는다.
	return grid[currentPosition.y][sideX] == 1 ||
		grid[sideY][currentPosition.x] == 1;
}

Node* AStarAlgorithm::FindOpenNode(int x, int y) const
{
	/* 파라미터로 넣은 좌표와 동일한 위치의 노드를 openList에서 찾는다 */
	for (Node* node : openList) if (node->position == Position(x, y)) return node;
	return nullptr;
}

bool AStarAlgorithm::IsInClosedList(int x, int y) const
{
	// 같은 좌표가 Closed 목록에 있는지 확인한다.
	for (Node* node : closedList)
	{
		//if (node->position.x == x && node->position.y == y)
		if (node->position == Position(x, y))
		{
			return true;
		}
	}

	return false;
}

bool AStarAlgorithm::IsDestination(const Node* node) const
{
	// 현재 노드와 목표 노드의 위치를 비교한다.
	return node != nullptr && goalNode != nullptr &&
		node->position == goalNode->position;
}

void AStarAlgorithm::ClearVisualization(std::vector<std::vector<int>>& grid) const
{
	// 탐색 후보 표시를 빈 공간으로 되돌린다.
	for (std::vector<int>& row : grid)
	{
		for (int& value : row)
		{
			if (value == 5)
			{
				value = 0;
			}
		}
	}
}

void AStarAlgorithm::DisplayGrid(const std::vector<std::vector<int>>& grid) const
{
	// 커서를 원점으로 이동해 같은 위치에 다시 출력한다.
	static COORD position = { 0, 0 };
	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

	int white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	int green = FOREGROUND_GREEN;
	int red = FOREGROUND_RED;

	for (int y = 0; y < static_cast<int>(grid.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(grid[y].size()); ++x)
		{
			if (grid[y][x] == 2)
			{
				SetConsoleTextAttribute(handle, red);
				std::cout << "S ";
			}
			else if (grid[y][x] == 3)
			{
				SetConsoleTextAttribute(handle, red);
				std::cout << "G ";
			}
			else if (grid[y][x] == 1)
			{
				SetConsoleTextAttribute(handle, white);
				std::cout << "1 ";
			}
			else if (grid[y][x] == 5)
			{
				SetConsoleTextAttribute(handle, green);
				std::cout << "+ ";
			}
			else
			{
				SetConsoleTextAttribute(handle, white);
				std::cout << "0 ";
			}
		}

		std::cout << "\n";
	}
}

void AStarAlgorithm::DisplayGridWithPath(
	std::vector<std::vector<int>>& grid,
	const std::vector<Position>& path)
{
	ClearVisualization(grid);
	DisplayGrid(grid);

	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int green = FOREGROUND_GREEN;

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

		SetConsoleCursorPosition(handle, consolePosition);
		SetConsoleTextAttribute(handle, green);
		std::cout << "* ";
		Sleep(50);
	}
}