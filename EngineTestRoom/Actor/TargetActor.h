#pragma once
#include <Actor/Actor.h>

class TargetActor : public AActor
{
    TYPE_DECLARATIONS(TargetActor, AActor)
    
public:
    TargetActor(const Vector3& location);
    
};
