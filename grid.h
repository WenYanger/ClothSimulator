#ifndef GRID_H
#define GRID_H


//
//  Grid.hpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/12.
//  Copyright © 2016年 WenYang. All rights reserved.
//


#include <iostream>
#include "Mass.h"
#include "Spring.h"



class Grid
{

public:
    Mass*** arr_mass;
    Mass*** arr_mass_TimeStepPrediction;
    GLfloat* arr_position;//存储所有点的位置的动态数组，用以绘制图形
    double m_timeStep;
    double m_critical_timeStep;
    bool timeStepAvailablityBuffer[3];//存储往前三个时刻timeStep是否合理

    Force* m_externalForce;

public:
    int N;
    int m_height,m_width;

    void next_state();
    double get_timeStep();
    double getProperTimeStep();
    double getProperTimeStep(double);
    bool checkTimeStep(double);
    void dynamicInverse();
    //int checkTimeStep(double);
    void updateTimeStepPredictionMassArray();
    void setElasticity(double);
    void setRange(double);
    GLfloat* get_position_array();

    //Constuctor
    Grid();
    Grid(int n_N, double mass,double timestep, Force externalForce);
    Grid(int width,int height, double mass,double timestep, Force externalForce);

};



#endif // GRID_H
