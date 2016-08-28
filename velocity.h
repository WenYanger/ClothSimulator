#ifndef VELOCITY_H
#define VELOCITY_H


#include <stdio.h>
#include "DefaultTypes.h"

class Velocity
{
public:
    vec3 m_velocity;

    Velocity();
    Velocity(vec3 velocity);

    Velocity operator+(const Velocity & velocity)
    {
        return m_velocity + velocity.m_velocity;
    }

    Velocity operator*(double factor)
    {
        return m_velocity * factor;
    }
};

#endif // VELOCITY_H
