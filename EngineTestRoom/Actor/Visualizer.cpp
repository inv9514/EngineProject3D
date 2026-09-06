#include "Visualizer.h"
#include <Component/MeshRendererComponent.h>

Visualizer::Visualizer(const Vector3& location)
    : super(location)
{
    meshRendererComponent = AddComponent<UMeshRendererComponent>();   
    SetRootComponent(meshRendererComponent);
     
    meshRendererComponent->SetMeshColor(Color::BrightYellow);
}

Visualizer::~Visualizer()
{
}
