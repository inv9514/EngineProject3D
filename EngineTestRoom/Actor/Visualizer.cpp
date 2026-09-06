#include "Visualizer.h"
#include <Component/MeshRendererComponent.h>

Visualizer::Visualizer(const Vector3& location)
    : super(location)
{
    meshRendererComponent = AddComponent<UMeshRendererComponent>();   
    SetRootComponent(meshRendererComponent);
     
    meshRendererComponent->SetMeshColor(Color::BrightYellow);
    SetActorScale(Vector3(0.5f, 0.5f, 0.5f));
    
    meshRendererComponent->SetIsHidden(false);
}

Visualizer::~Visualizer()
{
}

void Visualizer::SetMeshColor(Color color)
{
    meshRendererComponent->SetMeshColor(color); 
}
