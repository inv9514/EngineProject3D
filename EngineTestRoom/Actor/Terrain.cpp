#include "Terrain.h" 

#include <Component/MeshRendererComponent.h>
#include <Component/SphereComponent.h>   

Terrain::Terrain(const Vector3& location)
    : super(location)
{
    meshRendererComponent = AddComponent<UMeshRendererComponent>();    
    
    SetRootComponent(meshRendererComponent);
    meshRendererComponent->SetRelativeLocation(location);
}

Terrain::~Terrain()
{
    
}

void Terrain::Tick(float deltaTime)
{
    AActor::Tick(deltaTime);        
}