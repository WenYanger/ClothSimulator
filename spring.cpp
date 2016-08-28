//
//  Spring.cpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/11.
//  Copyright © 2016年 WenYang. All rights reserved.
//

#include "Spring.h"
#include "Mass.h"
#include <math.h>

//Implementation
Force* Spring::get_force()
{
    vec3 direction;
    double length = m_mass1->m_position.distanceFrom(m_mass2->m_position);
    double dLength = fabs(length - m_original_length);//求出形变的绝对值
    double factor = (1-(dLength/m_original_length)/m_range);//这个因数有可能小于0，这种情况是不允许的
    //double force = (dLength * m_elasticity)/(factor > 0 ? factor : 0.00001);
    if(factor<=0)
        std::cout<<"error"<<std::endl;
    double force = (dLength * m_elasticity)/factor;
    //double force =  dLength * m_elasticity;

    //如果在形变系数允许的范围内
    if(length<=((1+m_range)*m_original_length) && length>=((1-m_range)*m_original_length))
    {
        if( m_original_length <= length)//伸张状态
        {
            direction = (m_mass2->m_position - m_mass1->m_position).standarize();
            return new Force(direction * force);
        }
        else if( m_original_length >= length)//收缩状态
        {
            direction = (m_mass1->m_position - m_mass2->m_position).standarize();
            return new Force(direction * force);
        }
    }
    else
    {
        if( m_original_length < length)//伸张状态
        {
            direction = (m_mass2->m_position - m_mass1->m_position).standarize();
            return new Force(direction * force);
        }
        else if( m_original_length > length)//收缩状态
        {
            direction = (m_mass1->m_position - m_mass2->m_position).standarize();
            return new Force(direction * force);
        }
    }
}

double Spring::get_current_length()
{
    return m_mass1->m_position.distanceFrom(m_mass2->m_position);
}

//Construtor
Spring::Spring()
{
    m_original_length = 1;
    m_range = 1;
}

Spring::Spring(double length, double range)
{
    m_original_length = length;
    m_range = range;
}

Spring::Spring(Mass* mass1, Mass* mass2)
{
    m_original_length = mass1->m_position.distanceFrom(mass2->m_position);
    m_range = 0.4;
    m_elasticity = 1500 ;//弹性越差，布料越松
    m_stiffness = 10000;//硬度越大，变得越慢
    m_mass1 = mass1;
    m_mass2 = mass2;
}
