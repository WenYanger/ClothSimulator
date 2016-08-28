#ifndef FORCE_H
#define FORCE_H


#include <stdio.h>
#include "DefaultTypes.h"

class Force
{
public:
    vec3 m_force;

    Force();
    Force(vec3 force);

    Force operator+(const Force & force)
    {
        return Force(m_force + force.m_force);
    }

    vec3 operator/(double factor)
    {
        return m_force / factor;
    }
};

#endif // FORCE_H
