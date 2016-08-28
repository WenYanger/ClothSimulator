#ifndef MASS_H
#define MASS_H


//
//  Mass.hpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/11.
//  Copyright © 2016年 WenYang. All rights reserved.
//



#include <stdio.h>
#include "Spring.h"
#include "Force.h"
#include "Velocity.h"



class Mass
{

public:

    double m_mass;
    Force* m_force; //Contains previous Force, which can be overlapped by a new Force coming up
    Force* m_internal_force;
    Force* m_external_force;
    Velocity* m_velocity;
    vec3 m_position;
    bool stabled = false;

    Spring* arr_spring[12];
    double  arr_previous_length[12];//存储了该质点上一时刻所有Spring的长度，以便本时刻检查步长合理性

public:
    Force* internalForce();
    bool checkIfShiftOutOfRange();
    void updateSpringLength();


    //Constuctor
    Mass();
    Mass(double mass);
    Mass(double mass, Force force, Velocity* velocity, vec3 position);
};



#endif // MASS_H
