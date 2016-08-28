#ifndef SPRING_H
#define SPRING_H


//
//  Spring.hpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/11.
//  Copyright © 2016年 WenYang. All rights reserved.
//


#include <stdio.h>
#include "Force.h"
#include "DefaultTypes.h"

class Mass;

class Spring
{

public:

    double m_original_length;
    double m_elasticity;//弹性
    double m_range;//形变承受最大值
    double m_stiffness = 1;//硬度

    Mass* m_mass1;
    Mass* m_mass2;

public:

    Force* get_force();
    double get_current_length();

    Spring();
    Spring(double,double);
    Spring(Mass*, Mass*);
};



#endif // SPRING_H
