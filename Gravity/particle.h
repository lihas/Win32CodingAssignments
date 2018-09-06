#pragma once
#include<Windows.h>

 class _3DCoOrdinate
 {
 public:
     int x, y, z;
     _3DCoOrdinate()
     {
         x = y = z = 0;
     }
     _3DCoOrdinate(int x, int y, int z)
     {
         this->x = x;
         this->y = y;
         this->z = z;
     }
 };

 /*For strong typing*/
#define Make3DCordType(name)\
class name : public _3DCoOrdinate { };

 Make3DCordType(Position);
 Make3DCordType(Velocity);
 Make3DCordType(Acceleration);
 Make3DCordType(Color); //RGB

class Particle
{
    Position m_position;
    Velocity m_velocity;
    Acceleration m_acceleration;
    Color color = {0,0,255};
    float m_mass = 0;

public:
    Particle();
    Particle(Position position);
    ~Particle();
    bool SetPosition(Position position);
    int draw(HDC hDc);
};

