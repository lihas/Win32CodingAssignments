#include "Physics.h"
#include <cmath>

float Physics::m_G = 01.001;

Physics::Physics()
{
}


Physics::~Physics()
{
}

Force Physics::CalculateGravitationalForce(Particle p1, Particle p2)
{
    Force f;
    Displacement r = CalculateDistance(p1.GetPosition(), p2.GetPosition());
    f = CalculateGravitationalForce(p1.GetMass(), p2.GetMass(), r);
    return f;
}

Force Physics::CalculateGravitationalForce(int mass1, int mass2, Displacement r)
{
    Force f;
    f.x = r.x ? (-1 * r.x / abs(r.x))*(m_G*(mass1*mass2)) / abs(pow(r.x, 1)) : 0;
    f.y = r.y ? (-1 * r.y / abs(r.y))*(m_G*(mass1*mass2)) / abs(pow(r.y, 1)) : 0;
    f.z = r.z ? (1 * r.z / abs(r.z))*(m_G*(mass1*mass2)) /  abs(pow(r.z, 1)) : 0;
    return f;
}

Displacement Physics::CalculateDistance(Position p1, Position p2)
{
    Displacement d;
    d.x = p1.x - p2.x;
    d.y = p1.y - p2.y;
    d.z= p1.z - p2.z;
    return d;
}
