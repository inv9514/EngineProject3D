#pragma once
#include <World/World.h>
#include "Actor/Player.h"
#include "NavigationSystem/Position.h"
#include <Util/Timer.h>

class Position;
class Target;
class Stalker;

class World02 : public UWorld
{
    TYPE_DECLARATIONS(World02, UWorld)

public:
    void OnInitialized() override;
    
    void Tick(float deltaTime) override;
    
    void CreatePreset();
    
private:
    bool FindStartAndGoalPosition(const std::vector<std::vector<int>>& grid, Position& outStartPosition, Position& outGoalPosition);
    
private:
    std::shared_ptr<Player> player;   
    
    std::shared_ptr<Stalker> stalker1;
    std::shared_ptr<Stalker> stalker2;
    
    std::shared_ptr<Target> target;
    
private:
    std::vector<Position> visualizerPositions;
    size_t visualizerIndex = 0;

    StatTimer visualizerTimer{"AStar Visualization"};
};
