#include "Stalker.h"

#include <Component/MeshRendererComponent.h>
#include <Component/SphereComponent.h>

Stalker::Stalker(const Vector3& location)
    : super(location)
{
    sphereComponent = AddComponent<USphereComponent>();
    meshRendererComponent = AddComponent<UMeshRendererComponent>();   
    SetRootComponent(sphereComponent);
     
    meshRendererComponent->SetRelativeLocation(location);    
    meshRendererComponent->SetMeshColor(Color::BrightRed);
}

Stalker::~Stalker()
{
}

void Stalker::Tick(float deltaTime)
{
    AActor::Tick(deltaTime);
}

void Stalker::OnCollision(const std::shared_ptr<AActor>& other)
{
    AActor::OnCollision(other);
}
