#pragma once
#include <World/World.h>

#include "Actor/CubeActor.h"
#include "Actor/PlayerActor.h"

class StalkerActor;

class TestWorld : public UWorld
{
    TYPE_DECLARATIONS(TestWorld, UWorld)

public:
    void OnInitialized() override;
    
    void Tick(float deltaTime) override;
    
private:
    std::shared_ptr<PlayerActor> playerActor;   
    
    std::shared_ptr<StalkerActor> stalker1;
    std::shared_ptr<StalkerActor> stalker2;
};
