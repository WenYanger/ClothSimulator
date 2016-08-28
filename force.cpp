#include "force.h"

Force::Force()
{
    m_force = vec3{0,0,0};
}

Force::Force(vec3 force)
{
    m_force = force;
}
