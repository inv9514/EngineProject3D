#include "StalkerActor.h"

#include <Component/MeshRendererComponent.h>
#include <Component/SphereComponent.h>

StalkerActor::StalkerActor(const Vector3& location)
    : super(location)
{
    sphereComponent = AddComponent<USphereComponent>();
    SetRootComponent(meshRendererComponent);
    
    meshRendererComponent = AddComponent<UMeshRendererComponent>();    
    meshRendererComponent->SetRelativeLocation(location);
    
    meshRendererComponent->SetMeshColor(Color::BrightRed);
}

StalkerActor::~StalkerActor()
{
}

void StalkerActor::Tick(float deltaTime)
{
    AActor::Tick(deltaTime);
}

void StalkerActor::OnCollision(const std::shared_ptr<AActor>& other)
{
    AActor::OnCollision(other);
}
