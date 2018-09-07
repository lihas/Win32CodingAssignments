#pragma once

#include"particle.h"
#include<random>

constexpr int maxParticles = 20;


class World
{
    const int m_windowHeight = 1080;
    const int m_windowWidth = 1920;

    Particle m_particles[maxParticles];

    std::random_device rDevice;
    std::mt19937 pseudoRandomDeviceX{ rDevice() };
    std::mt19937 pseudoRandomDeviceY{ rDevice() };
    std::uniform_int_distribution<int> uniformDistX{0, m_windowWidth};
    std::uniform_int_distribution<int> uniformDistY{ 0, m_windowHeight };

    int generateUniformX();
    int generateUniformY();
public:
    

    int GetWindowHeight();
    int GetWindowWidth();
    World();
    ~World();
    int draw(HDC hDc);
    int ScaleDown(HDC hDC);
private:
    // //advance one unit of time in his world
    int CalculateStep();
};

