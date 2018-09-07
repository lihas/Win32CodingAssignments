#pragma once
#include<Windows.h>
#include "Common.h"
#include <vector>
 

class Particle
{
    Position m_position;
    Position m_PrevPosition;
    std::vector<Position> m_PrevPositions;
    Velocity m_velocity;
    Color m_color {0,0,255};
    Force m_force{0,0,0};
    HBRUSH m_fillBrush;
    float m_width, m_height;
    float m_mass = 100;

public:
    HPEN m_pen;
    Particle();
    Particle(Position position);
    ~Particle();
    float GetMass();
    Position GetPosition();
    bool SetPosition(Position position);
    int draw(HDC hDc);
    int addForce(Force);
    int step(); //move one unit of time given force, mass, position, and current velocity
};

