#include "velocity.h"

Velocity::Velocity()
{
    m_velocity = vec3{0,0,0};
}

Velocity::Velocity(vec3 velocity)
{
    m_velocity = velocity;
}

