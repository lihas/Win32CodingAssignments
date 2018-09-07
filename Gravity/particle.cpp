#include "particle.h"



Particle::Particle()
{
    m_height = 10;
    m_width = 10;
    m_fillBrush = CreateSolidBrush(RGB(m_color.x, m_color.y, m_color.z));
}

Particle::Particle(Position position)
{
    m_position = position;
    m_PrevPosition = position;
    m_fillBrush = CreateSolidBrush(RGB(m_color.x, m_color.y, m_color.z));
}

bool Particle::SetPosition(Position position)
{
    m_position = position;
    m_PrevPosition = m_position;
    return true;
}

Position Particle::GetPosition()
{
    return m_position;
}

float Particle::GetMass()
{
    return m_mass;
}

int Particle::addForce(Force f)
{
    m_force.x += f.x;
    m_force.y += f.y;
    m_force.z += f.z;
    return 0;
}

Particle::~Particle()
{
    DeleteObject(m_fillBrush);
}

int i = 0;

int Particle::draw(HDC hDc)
{
    int top = (m_position.y - m_height / 2);
    int left = (m_position.x - m_width / 2);
    int bottom = (m_position.y + m_height / 2);
    int right = (m_position.x + m_width / 2);
    
    auto oldBrush = (HBRUSH)SelectObject(hDc, m_fillBrush);

    HPEN oldPen = (HPEN)SelectObject(hDc, m_pen);
    //Ellipse(hDc, left, top, right, bottom);
    auto firstPos = m_PrevPositions[0];
    MoveToEx(hDc, firstPos.x, firstPos.y, NULL);
    auto i = m_PrevPositions.begin();
    i++;
    for (; i < m_PrevPositions.end(); i++)
    {
        LineTo(hDc, i->x, i->y);
    }
    /*RECT rc = {0,0,30,30};
    
    DrawText(hDc, "{0,0,30,30}", -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

    rc = { 40,40,70,70 };

    DrawText(hDc, "{ 40,40,70,70 }", -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);*/

    SelectObject(hDc, oldBrush);
    SelectObject(hDc, oldPen);
    return 0;
}


int Particle::step()
{
    Acceleration a;
    if (m_mass == 0 || m_force == Force{0,0,0})
    {
        return 0;//particle will not move
    }

    a.x = m_force.x / m_mass;
    a.y = m_force.y / m_mass;
    a.z = m_force.z / m_mass;

    auto dampAccel = m_velocity*0.5;//damp acceleration with velocity
    dampAccel.x = abs(dampAccel.x);
    dampAccel.y = abs(dampAccel.y);
    dampAccel.z = abs(dampAccel.z);

    Velocity vfinal;
    vfinal.x = m_velocity.x + a.x; //v= u + at
    vfinal.y = m_velocity.y + a.y;
    vfinal.z = m_velocity.z + a.z;

    Displacement d;

    d.x = m_velocity.x + 0.5*a.x; // s= ut + 0.5at^2
    d.y = m_velocity.y + 0.5*a.y;
    d.z = m_velocity.z + 0.5*a.z;

    m_PrevPosition = m_position;
    m_PrevPositions.push_back(m_PrevPosition);

    m_position.x += d.x;
    m_position.y += d.y;
    m_position.z += d.z;

    m_velocity = vfinal;
    return 0;
}