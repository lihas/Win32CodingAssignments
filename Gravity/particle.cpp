#include "particle.h"



Particle::Particle()
{
}

Particle::Particle(Position position)
{
    m_position = position;
}

bool Particle::SetPosition(Position position)
{
    m_position = position;
    return true;
}
Particle::~Particle()
{
}


int Particle::draw(HDC hDc)
{
    return 0;
}
