#include "TestWorld.h"

#include <iostream>
#include <Render/Renderer3D.h>
#include <World/World.h>
#include <Actor/Player.h>
#include "Actor/Terrain.h"
#include "Actor/Stalker.h"
#include "Actor/Target.h"
#include "AI/AStarAlgorithm.h"
#include "AI/Position.h"
#include "Actor/Visualizer.h"


void TestWorld::OnInitialized()
{    
    UWorld::OnInitialized();    
    
    /* Preset1 */
    CreatePreset();
    
    /* Player (Camera) */
    SpawnActor<Player>(Vector3(-10, 0, 0));
}

void TestWorld::Tick(float deltaTime)
{
    UWorld::Tick(deltaTime);    
}

void TestWorld::CreatePreset()
{    
    std::vector<std::vector<int>> grid =
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 1 },
        { 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
        { 1, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
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
    
    Position startPosition;
    Position goalPosition;
    
    
    AStarAlgorithm aStar;
    
    
    std::vector<Position> path = aStar.FindPath(startPosition, goalPosition, grid, this);
    if (!path.empty())
    {
        
    }
    else
    {
        
    }    
}
