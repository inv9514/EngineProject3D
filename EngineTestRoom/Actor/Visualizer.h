#pragma once
#include <Actor/Actor.h>
#include <Component/MeshRendererComponent.h>

#include "NavigationSystem/Position.h"

class UMeshRendererComponent;

class Visualizer : public AActor
{
    TYPE_DECLARATIONS(Visualizer, AActor)

public:
    Visualizer(const Vector3& location);
    ~Visualizer();
    
    void SetGridPosition(const Position& position) { gridPosition = position; }

    const Position& GetGridPosition() const { return gridPosition; }
    
    void SetMeshColor(Color color);
    
private:
    std::shared_ptr<UMeshRendererComponent> meshRendererComponent;
    
    Position gridPosition;
};
