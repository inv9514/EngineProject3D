#pragma once
#include <World/World.h>

#include "Actor/Terrain.h"
#include "Actor/Player.h"

class Target;
class Stalker;

class TestWorld : public UWorld
{
    TYPE_DECLARATIONS(TestWorld, UWorld)

public:
    void OnInitialized() override;
    
    void Tick(float deltaTime) override;
    
    void CreatePreset();
    
private:
    std::shared_ptr<Player> playerActor;   
    
    std::shared_ptr<Stalker> stalker1;
    std::shared_ptr<Stalker> stalker2;
    
    std::shared_ptr<Target> target;
};
