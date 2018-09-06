#pragma once

#include"particle.h"
#include<random>

constexpr int maxParticles = 100;
constexpr int windowHeight = 640;
constexpr int windowWidth  = 480;

class World
{
    Particle m_particles[maxParticles];

    std::random_device rDevice;
    std::mt19937 pseudoRandomDeviceX{ rDevice() };
    std::mt19937 pseudoRandomDeviceY{ rDevice() };
    std::uniform_int_distribution<int> uniformDistX{0, windowWidth};
    std::uniform_int_distribution<int> uniformDistY{ 0, windowHeight };

    int generateUniformX();
    int generateUniformY();
public:
    World();
    ~World();
};

