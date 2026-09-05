#include "CubeActor.h" 

#include <Component/MeshRendererComponent.h>
#include <Component/SphereComponent.h>   

CubeActor::CubeActor(const Vector3& location)
    : super(location)
{
    meshRendererComponent = AddComponent<UMeshRendererComponent>();    
    
    SetRootComponent(meshRendererComponent);
    meshRendererComponent->SetRelativeLocation(location);
}

CubeActor::~CubeActor()
{
    
}

void CubeActor::Tick(float deltaTime)
{
    AActor::Tick(deltaTime);        
}