
//
//  Mass.cpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/11.
//  Copyright © 2016年 WenYang. All rights reserved.
//

#include "Mass.h"
#include <math.h>


//Implementation
Force* Mass::internalForce()
{
    Force* f = new Force();
    return f;
}

//如果有任何一个Spring的shift过长了，就返回0，如果在长度95%以内返回2，在长度95%以上返回1
//int Mass::checkIfShiftOutOfRange()
//{
//    double factor = 0.001;//控制因子
//    double stiffness;
//    double shift;
//    for(int i=0;i<=7;i++)
//    {
//        if(arr_spring[i] !=0 ){
//            shift = arr_spring[i]->get_current_length() - arr_spring[i]->m_original_length;
//            stiffness = arr_spring[i]->m_stiffness;
//            shift = shift > 0 ? shift : -shift;
//            if(shift >= arr_spring[i]->m_original_length * stiffness){
//                return 0;
//            }else if(shift >= arr_spring[i]->m_original_length * stiffness * 0.95 && shift < arr_spring[i]->m_original_length * stiffness){
//                return 1;
//            }
//        }
//    }
//    return 2;
//}

bool Mass::checkIfShiftOutOfRange()
{
    double factor = 0.001;//控制因子
    double range;
    double shift;
    for(int i=0;i<=11;i++)
    {
        if(arr_spring[i] !=0 ){
            if(i>=0&&i<=3)
                range = arr_spring[i]->m_range;
            else if(i>=4&&i<=7)
                range = arr_spring[i]->m_range;
            else if(i>=8&&i<=11)
                range = arr_spring[i]->m_range;
            shift = arr_spring[i]->get_current_length() - arr_spring[i]->m_original_length;
            shift = shift > 0 ? shift : -shift;
            if(shift >= arr_spring[i]->m_original_length * range){
                return true;
            }
        }
    }
    return false;
}

//bool Mass::checkIfShiftOutOfRange()
//{
//    double factor = 0.001;//控制因子
//    double shift;
//    for(int i=0;i<=11;i++)
//    {
//        if(arr_spring[i] !=0 ){
//            shift = arr_spring[i]->get_current_length() - arr_previous_length[i];
//            shift = shift > 0 ? shift : -shift;
//            if(shift > arr_spring[i]->m_original_length * factor){
//                return true;
//            }
//        }
//    }
//    return false;
//}

//更新存储上一时刻Spring的长度
void Mass::updateSpringLength()
{
    for(int i=0;i<=11;i++)
    {
        if(arr_spring[i]!=0){
            arr_previous_length[i] = arr_spring[i]->get_current_length();
        }
    }
}

//Constructor
Mass::Mass()
{
    m_mass = 1;
    m_force = new Force(vec3{0,0,0});
    m_position = vec3({0,0,0});
    m_velocity =  new Velocity(vec3{0,0,0});

    //初始化spring指针为空
    int i;
    for(i=0;i<12;i++)
    {
        arr_spring[i] = 0;
    }
}

Mass::Mass(double mass)
{

    //初始化spring指针为空
    int i;
    for(i=0;i<12;i++)
    {
        arr_spring[i] = 0;
    }

    m_mass = mass;
    m_force = new Force(vec3{0,0,0});
    m_external_force = new Force(vec3{0,0,0});
    m_position = vec3({0,0,0});
    m_velocity = new Velocity(vec3{0,0,0});
}

Mass::Mass(double mass, Force force, Velocity* velocity, vec3 position)
{


    int i;
    for(i=0;i<12;i++)
    {
        //初始化spring指针为空
        arr_spring[i] = 0;
        //初始化spring长度
        if(i>=0&&i<=3)
            arr_previous_length[i] = 1.0;
        else if(i>=4&&i<=7)
            arr_previous_length[i] = sqrt(2);
        else if(i>=8&&i<=11)
            arr_previous_length[i] = 2;
    }


    m_mass = mass;
    m_force = new Force(force.m_force);
    m_external_force = new Force(force.m_force);
    m_position = position;
    m_velocity = velocity;
}
