#pragma once
#include "particle.h"
#include "Common.h"

class Physics
{
    static float m_G; //gravitational constant
public:
    static Force CalculateGravitationalForce(Particle p1, Particle p2);
    static Force CalculateGravitationalForce(int mass1, int mass2, Displacement r);
    Physics();
    ~Physics();
    static Displacement CalculateDistance(Position p1, Position p2);
};

