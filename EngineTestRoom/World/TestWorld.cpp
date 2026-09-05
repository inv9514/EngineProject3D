#include "TestWorld.h"
#include <Render/Renderer3D.h>
#include <World/World.h>
#include <Actor/PlayerActor.h>
#include "Actor/CubeActor.h"

void TestWorld::OnInitialized()
{    
    UWorld::OnInitialized();
    
    SpawnActor<PlayerActor>(Vector3(-10, 0, 0));
    
    cube1 = SpawnActor<CubeActor>(Vector3(0, 0, 5));
    cube2 = SpawnActor<CubeActor>(Vector3(0, 0, 0));
    
    
}

void TestWorld::Tick(float deltaTime)
{
    UWorld::Tick(deltaTime);
    
    cube2->SetActorLocation(cube2->GetActorLocation() + Vector3(0,0,0.01f));
}
