#pragma once
#include <Actor/Actor.h>

class USphereComponent;
class UMeshRendererComponent;

class CubeActor : public AActor
{
    TYPE_DECLARATIONS(CubeActor, AActor)

public:
    CubeActor(const Vector3& location);
    
    void Tick(float deltaTime) override;
    
    void OnCollision(const std::shared_ptr<AActor>& other) override;
    
private:
    std::shared_ptr<UMeshRendererComponent> meshRendererComponent;
    std::shared_ptr<USphereComponent> sphereComponent;
};
