#include "CubeActor.h"
#include <Component/MeshRendererComponent.h>
#include <Component/SphereComponent.h>   

CubeActor::CubeActor(const Vector3& location)
: AActor(location)
{
    meshRendererComponent = AddComponent<UMeshRendererComponent>();    
    sphereComponent = AddComponent<USphereComponent>();
    
    SetRootComponent(meshRendererComponent);
}

void CubeActor::Tick(float deltaTime)
{
    AActor::Tick(deltaTime);    
    
}

void CubeActor::OnCollision(const std::shared_ptr<AActor>& other)
{
    AActor::OnCollision(other);
    Destroy();
}
