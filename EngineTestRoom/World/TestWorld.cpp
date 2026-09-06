#include "TestWorld.h"
#include <Render/Renderer3D.h>
#include <World/World.h>
#include <Actor/Player.h>
#include "Actor/Terrain.h"
#include "Actor/Stalker.h"
#include "Actor/Target.h"

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
    /* Map */
    bool blocks[10][5][20] {};   
    
    /* 일단 바닥은 전부 생성 */
    for (int i = 0; i < 10; i++)
    {
        for (int k = 0; k < 20; k++)
        {
            blocks[i][0][k] = true;
        }
    }    
    
    /* 바닥에 구멍좀 뚫고 */
    blocks[3][1][8] = false;
    blocks[4][1][9] = false;
    blocks[8][1][2] = false;
    blocks[3][1][5] = false;
    
    /* 1층 */
    
    /* 2층 */

    
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
    
    /*

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 20; k++)
            {
                if (blocks[i][j][k] == true) SpawnActor<Terrain>(Vector3(static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)));
            }
        }
    }
    */
}
