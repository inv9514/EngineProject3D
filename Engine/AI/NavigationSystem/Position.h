#pragma once

class Position
{
public:
    Position(int x = 0, int y = 0)
        : x(x), y(y)
    {
        
    }
    
    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }
    
public:
    int x;
    int y;
};
