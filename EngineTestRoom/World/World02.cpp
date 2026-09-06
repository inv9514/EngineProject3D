#include "World02.h"

#include <iostream>
#include <Render/Renderer3D.h>
#include <World/World.h>
#include <Actor/Player.h>
#include "Actor/Terrain.h"
#include "Actor/Stalker.h"
#include "Actor/Target.h"
#include "NavigationSystem/AStarAlgorithm.h"
#include "Actor/Visualizer.h"


void World02::OnInitialized()
{    
    UWorld::OnInitialized();    
    
    
    /* Player (Camera) */
    player = SpawnActor<Player>(Vector3(0, 2, 0));    
    player->SetActorRotation(Vector3(0,0,0));
    
    /* Preset1 */
    CreatePreset();
}

void World02::Tick(float deltaTime)
{
    UWorld::Tick(deltaTime);    
    
    if (visualizerIndex >= visualizerPositions.size()) return;

    visualizerTimer.CheckEndStat();

    if (visualizerTimer.GetElapsed() < 0.5f) return;

    const Position& position = visualizerPositions[visualizerIndex];

    SpawnActor<Visualizer>(Vector3(static_cast<float>(position.y),1.0f,static_cast<float>(position.x)));

    ++visualizerIndex;

    visualizerTimer.CheckStartStat();
}

void World02::CreatePreset()
{    
    /* 그리드를 설정하고 액터들 생성 */
    std::vector<std::vector<int>> grid =
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 1 },
        { 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
        { 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };
    
    for (int x = 0; x < static_cast<int>(grid.size()); ++x)
    {
        for (int z = 0; z < static_cast<int>(grid[x].size()); ++z)
        {
            if (grid[x][z] == 1) continue;
            SpawnActor<Terrain>(Vector3{static_cast<float>(x),0.0f,static_cast<float>(z)});
            if (grid[x][z] == 2) SpawnActor<Stalker>(Vector3{static_cast<float>(x),1.0f,static_cast<float>(z)});
            if (grid[x][z] == 3) SpawnActor<Target>(Vector3{static_cast<float>(x),1.5f,static_cast<float>(z)});
        }
    }
    
    /* A-Star 알고리즘 실행 */
    Position startPosition;
    Position goalPosition;
    
    FindStartAndGoalPosition(grid, startPosition, goalPosition);    
    AStarAlgorithm aStar;        
    std::vector<Position> path = aStar.FindPath(startPosition, goalPosition, grid);
    
    /* 알고리즘 실행결과로 나온 Visualizer 위치목록을 받아 하나씩 생성 */
    visualizerPositions = aStar.GetVisualizerPosition();
    visualizerIndex = 0;
    visualizerTimer.CheckStartStat();
}


bool World02::FindStartAndGoalPosition(const std::vector<std::vector<int>>& grid, Position& outStartPosition, Position& outGoalPosition)
{
    if (grid.empty() || grid[0].empty())
    {
        return false;
    }

    // 시작점과 목표점의 중복 여부를 확인한다.
    bool foundStart = false;
    bool foundGoal = false;

    for (int y = 0; y < static_cast<int>(grid.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(grid[y].size()); ++x)
        {
            if (grid[y][x] == 2)
            {
                // 시작 위치는 정확히 하나만 허용한다.
                if (foundStart)
                {
                    return false;
                }

                outStartPosition = Position(x, y);
                foundStart = true;
            }
            else if (grid[y][x] == 3)
            {
                // 목표 위치도 정확히 하나만 허용한다.
                if (foundGoal)
                {
                    return false;
                }

                outGoalPosition = Position(x, y);
                foundGoal = true;
            }
        }
    }

    return foundStart && foundGoal;
}