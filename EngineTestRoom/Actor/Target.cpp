#include "Target.h"

#include <Component/MeshRendererComponent.h>
#include <Component/SphereComponent.h>

Target::Target(const Vector3& location)
: super(location)
{
    sphereComponent = AddComponent<USphereComponent>();
    meshRendererComponent = AddComponent<UMeshRendererComponent>();   
    SetRootComponent(meshRendererComponent);
     
    sphereComponent->SetRelativeLocation(location);
    meshRendererComponent->SetMeshColor(Color::BrightGreen);
    
}

Target::~Target()
{
}

void Target::Tick(float deltaTime)
{
    AActor::Tick(deltaTime);
    SetActorRotation(GetActorRotation() + Vector3(2.f,2.f,0.f));
}

void Target::OnCollision(const std::shared_ptr<AActor>& other)
{
    AActor::OnCollision(other);
}
