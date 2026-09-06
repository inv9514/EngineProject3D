#pragma once
#include <Actor/Actor.h>

class UMeshRendererComponent;

class Visualizer : public AActor
{
    TYPE_DECLARATIONS(Visualizer, AActor)

public:
    Visualizer(const Vector3& location);
    ~Visualizer();
    
private:
    std::shared_ptr<UMeshRendererComponent> meshRendererComponent;
};
