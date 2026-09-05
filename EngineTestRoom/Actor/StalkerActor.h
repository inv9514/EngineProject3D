#pragma once
#include <Actor/Actor.h>

class USphereComponent;
class UMeshRendererComponent;

class StalkerActor : public AActor
{
    TYPE_DECLARATIONS(StalkerActor, AActor)

public:
    StalkerActor(const Vector3& location);
    ~StalkerActor();
    
    void Tick(float deltaTime);   
    void OnCollision(const std::shared_ptr<AActor>& other) override;
    
private:
    std::shared_ptr<UMeshRendererComponent> meshRendererComponent;
    std::shared_ptr<USphereComponent> sphereComponent;
};
