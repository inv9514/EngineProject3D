#pragma once
#include <Core/Core.h>
#include "PrimitiveComponent.h"
#include "Math/Color.h"
#include "Shape/Mesh.h"

class ENGINE_API UMeshRendererComponent : public UPrimitiveComponent
{
public:
    TYPE_DECLARATIONS(UMeshRendererComponent, UPrimitiveComponent)

public:
    UMeshRendererComponent();
    virtual ~UMeshRendererComponent() = default;

public:
    virtual void Draw() override;

    const Mesh& GetMesh() const { return mesh; }

    void SetMesh(const Mesh& newMesh) { mesh = newMesh; }
    
    void SetMeshColor(const Color& newColor) { meshColor = newColor; }
    
    void SetIsHidden(bool newHidden) { isHidden = newHidden; }
    
    bool IsHidden() const { return isHidden; }

private:
    Mesh mesh;
    
    Color meshColor = Color::White;
    
    bool isHidden = false;
};
