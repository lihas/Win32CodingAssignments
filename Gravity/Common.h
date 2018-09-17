#pragma once
class _3DCoOrdinate
{
public:
    float x, y, z;
    _3DCoOrdinate()
    {
        x = y = z = 0;
    }
    _3DCoOrdinate(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    _3DCoOrdinate operator*(float factor)
    {
        _3DCoOrdinate x;
        x.x *= factor;
        x.y *= factor;
        x.z *= factor;
        return x;
    }
    bool operator==(_3DCoOrdinate other)
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }
};

/*For strong typing*/
#define Make3DCordType(_name)\
class _name : public _3DCoOrdinate { public:\
_name() = default;\
_name(float x, float y, float z):_3DCoOrdinate(x, y, z)\
 {\
 }\
 };

Make3DCordType(Position);
Make3DCordType(Displacement);
Make3DCordType(Velocity);
Make3DCordType(Acceleration);
Make3DCordType(Force);
Make3DCordType(Color); //RGB

