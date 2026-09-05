#include "TestWorld.h"
#include <Render/Renderer3D.h>
#include <World/World.h>
#include <Actor/PlayerActor.h>
#include "Actor/CubeActor.h"
#include "Actor/StalkerActor.h"

void TestWorld::OnInitialized()
{    
    UWorld::OnInitialized();
    
    /* Player (Camera) */
    SpawnActor<PlayerActor>(Vector3(-10, 0, 0));

    /* Field */
    for (int i = 0; i <10; i++)
    {
        for (int j = 0; j < 20; j++)
        {            
            SpawnActor<CubeActor>(Vector3(i, -10, j));
        }
    }
    
    /* Stalker */
    stalker1 = SpawnActor<StalkerActor>(Vector3(0, 0, 5));
    stalker2 = SpawnActor<StalkerActor>(Vector3(0, 0, 0));
    
    /* Target */
}

void TestWorld::Tick(float deltaTime)
{
    UWorld::Tick(deltaTime);    
}