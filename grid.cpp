

//
//  Grid.cpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/12.
//  Copyright © 2016年 WenYang. All rights reserved.
//

#include "Grid.h"
#include <math.h>

//Implementation
void Grid::next_state()
{
    int i,j,k = 0;
    Velocity t_v0, t_v1; //v0表示加速前速度，v1表示加速后速度
    vec3 t_acceleration;
    vec3 shifting;

    m_timeStep = getProperTimeStep(m_timeStep);
    //dynamicInverse();

    //Step 1:依照上一状态受力情况，计算下一状态
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            if(i==0&&j==0)
            {int ttt =1;}
            t_v0 = *arr_mass[i][j]->m_velocity;

            t_acceleration = arr_mass[i][j]->m_force->m_force / arr_mass[i][j]->m_mass;//F = ma计算得出加速度

            t_v1 = t_v0 + t_acceleration * m_timeStep;

            shifting = t_v1.m_velocity * m_timeStep;

            if(!arr_mass[i][j]->stabled){
                arr_mass[i][j]->m_velocity->m_velocity = t_v1.m_velocity;
                arr_mass[i][j]->m_position = arr_mass[i][j]->m_position+shifting;
            }

            arr_mass[i][j]->updateSpringLength();
            //std::cout<<arr_mass[i][j]->m_position<<std::endl;

        }
    }
    //Step 2:依照新状态，计算受力，并存储
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            Mass* mass = arr_mass[i][j];
            Force force = Force();
            //通过Spring遍历获得Mass收到的内力的总和
            for(k = 0; k < 12; k++)
            {
                if(mass->arr_spring[k] != 0 && k<=11)//先排除flexion spring
                {
                    force = force + *mass->arr_spring[k]->get_force();
                }
            }
            //根据当前速度计算阻尼，加入到力中
            force.m_force = force.m_force - (mass->m_velocity->m_velocity * 9);

            if(!mass->stabled)
                mass->m_force->m_force = force.m_force + mass->m_external_force->m_force;
            else
                mass->m_force->m_force = vec3{0,0,0};
                //mass->m_force->m_force = vec3{-force.m_force.x,-force.m_force.y,-force.m_force.z} + mass->m_external_force->m_force;
        }
    }
    //Step 3:更新预测Mass数组
    updateTimeStepPredictionMassArray();

}

GLfloat* Grid::get_position_array()
{
    GLfloat* array = arr_position;
    int i,j;
    double x,y,z;

    for(i=0;i<N*N;i++)//遍历所有mass指针
    {
        Mass * m = arr_mass[i/N][i%N];
        vec3 p = m->m_position;


        x = p.x;
        y = p.y;
        z = p.z;
        for(j=0;j<3;j++)
        {
            if(j == 0)
                array[i*3 + j] = (GLfloat)x;
            else if(j == 1)
                array[i*3 + j] = (GLfloat)y;
            else if(j == 2)
                array[i*3 + j] = (GLfloat)z;
            else
                printf("error\n");
        }
    }

    return array;
}

double Grid::get_timeStep()
{
    return m_timeStep;
}

//double Grid::getProperTimeStep(double t_timeStep)//该方法采用递归方法计算时间步长
//{
//    double origin_timeStep = t_timeStep;
//    double factor = 0.001;
//    int result;
//    static bool flag = true;//是否小于95%
//    bool isTimeStepChanged = false;

//    //Step 1:根据Buffer，判断是否变动timeStep
//    if(timeStepAvailablityBuffer[0]&&timeStepAvailablityBuffer[1]&&timeStepAvailablityBuffer[2] == true)
//    {
//        //如果三次计算成功，将步长变大
//        t_timeStep = t_timeStep + t_timeStep * factor;
//        isTimeStepChanged = true;
//    }

//    if(!isTimeStepChanged)//如果是原步长
//    {
//        result = checkTimeStep(t_timeStep);
//        if(result==0)//原步长不通过，步长减小，buffer清零
//        {
//            t_timeStep = t_timeStep - t_timeStep * factor;
//            timeStepAvailablityBuffer[0] = false;
//            timeStepAvailablityBuffer[1] = false;
//            timeStepAvailablityBuffer[2] = false;
//            return getProperTimeStep(t_timeStep);
//        }else if(result==1 && flag)//原步长大于95%，步长减小50%
//        {
//            flag = false;
//            t_timeStep = t_timeStep - t_timeStep * 0.9;
//            timeStepAvailablityBuffer[0] = false;
//            timeStepAvailablityBuffer[1] = false;
//            timeStepAvailablityBuffer[2] = false;
//            return getProperTimeStep(t_timeStep);
//        }else//原步长通过，更新buffer，返回原步长
//        {
//            flag = true;
//            //TODO 更新timeStepAvailablityBuffer状态
//            if(timeStepAvailablityBuffer[0] == false)
//            {
//                timeStepAvailablityBuffer[0] = true;
//            }else if(timeStepAvailablityBuffer[1] == false)
//            {
//                timeStepAvailablityBuffer[1] = true;
//            }else if(timeStepAvailablityBuffer[2] == false)
//            {
//                timeStepAvailablityBuffer[2] = true;
//            }
//            return t_timeStep;
//        }
//    }else
//    {
//        result = checkTimeStep(t_timeStep);
//        if(result==0)//变化步长不通过，恢复原步长，buffer清零
//        {
//            t_timeStep = origin_timeStep;
//            timeStepAvailablityBuffer[0] = false;
//            timeStepAvailablityBuffer[1] = false;
//            timeStepAvailablityBuffer[2] = false;
//            return getProperTimeStep(t_timeStep);
//        }else//变化步长通过，更新buffer，返回更新步长
//        {
//            if(t_timeStep >= m_critical_timeStep*0.00001)//如果比临界步长大
//            {
//                t_timeStep = origin_timeStep;
//                timeStepAvailablityBuffer[0] = false;
//                timeStepAvailablityBuffer[1] = false;
//                timeStepAvailablityBuffer[2] = false;
//                return getProperTimeStep(t_timeStep);
//            }
//            //更新timeStepAvailablityBuffer状态
//            if(timeStepAvailablityBuffer[0] == false)
//            {
//                timeStepAvailablityBuffer[0] = true;
//            }else if(timeStepAvailablityBuffer[1] == false)
//            {
//                timeStepAvailablityBuffer[1] = true;
//            }else if(timeStepAvailablityBuffer[2] == false)
//            {
//                timeStepAvailablityBuffer[2] = true;
//            }
//            return t_timeStep;
//        }
//    }

//}

double Grid::getProperTimeStep(double t_timeStep)//该方法采用递归方法计算时间步长
{
    double origin_timeStep = t_timeStep;
    double factor = 0.1;
    bool isTimeStepChanged = false;

    //Step 1:根据Buffer，判断是否变动timeStep
    if(timeStepAvailablityBuffer[0]&&timeStepAvailablityBuffer[1]&&timeStepAvailablityBuffer[2] == true)
    {
        //如果三次计算成功，将步长变大
        t_timeStep = t_timeStep + t_timeStep * factor;
        isTimeStepChanged = true;
    }

    if(!isTimeStepChanged)//如果是原步长
    {
        if(!checkTimeStep(t_timeStep))//原步长不通过，步长减小，buffer清零
        {
            t_timeStep = t_timeStep - t_timeStep * factor;
            timeStepAvailablityBuffer[0] = false;
            timeStepAvailablityBuffer[1] = false;
            timeStepAvailablityBuffer[2] = false;
            return getProperTimeStep(t_timeStep);
        }else//原步长通过，更新buffer，返回原步长
        {
            //TODO 更新timeStepAvailablityBuffer状态
            if(timeStepAvailablityBuffer[0] == false)
            {
                timeStepAvailablityBuffer[0] = true;
            }else if(timeStepAvailablityBuffer[1] == false)
            {
                timeStepAvailablityBuffer[1] = true;
            }else if(timeStepAvailablityBuffer[2] == false)
            {
                timeStepAvailablityBuffer[2] = true;
            }
            return t_timeStep;
        }
    }else
    {
        if(!checkTimeStep(t_timeStep))//变化步长不通过，恢复原步长，buffer清零
        {
            t_timeStep = origin_timeStep;
            timeStepAvailablityBuffer[0] = false;
            timeStepAvailablityBuffer[1] = false;
            timeStepAvailablityBuffer[2] = false;
            return getProperTimeStep(t_timeStep);
        }else//变化步长通过，更新buffer，返回更新步长
        {
            if(t_timeStep > m_critical_timeStep || t_timeStep > 0.02)//如果比临界步长大
            {
                t_timeStep = origin_timeStep;
                timeStepAvailablityBuffer[0] = false;
                timeStepAvailablityBuffer[1] = false;
                timeStepAvailablityBuffer[2] = false;
                return getProperTimeStep(t_timeStep);
            }
            //更新timeStepAvailablityBuffer状态
            if(timeStepAvailablityBuffer[0] == false)
            {
                timeStepAvailablityBuffer[0] = true;
            }else if(timeStepAvailablityBuffer[1] == false)
            {
                timeStepAvailablityBuffer[1] = true;
            }else if(timeStepAvailablityBuffer[2] == false)
            {
                timeStepAvailablityBuffer[2] = true;
            }
            return t_timeStep;
        }
    }

}

double Grid::getProperTimeStep()
{
    double t_timeStep = m_timeStep;
    bool isTimeStepChanged = false;

    //Step 1:在当前步长尝试运算下一状态，直到数值计算稳定为止
    if(!checkTimeStep(t_timeStep))//如果当前步长不合适，太大了
    {
        t_timeStep = t_timeStep * 0.125;
    }

    if(isTimeStepChanged)//如果步长改变
    {
        //TODO 更新timeStepAvailablityBuffer状态
        timeStepAvailablityBuffer[0] = false;
        timeStepAvailablityBuffer[1] = false;
        timeStepAvailablityBuffer[2] = false;


    }else
    {
        //TODO 检查timeStepAvailablityBuffer状态
        if(timeStepAvailablityBuffer[0]&&timeStepAvailablityBuffer[1]&&timeStepAvailablityBuffer[2] == true)
        {
            //如果三次计算成功，将步长变大
            t_timeStep = t_timeStep + t_timeStep * 0.1;
            //如果变大后不合理，恢复原步长
            if(!checkTimeStep(t_timeStep))
            {
                t_timeStep = t_timeStep - t_timeStep * 0.1;
            }

        }
        //TODO 更新timeStepAvailablityBuffer状态
        if(timeStepAvailablityBuffer[0] == false)
        {
            timeStepAvailablityBuffer[0] = true;
        }else if(timeStepAvailablityBuffer[1] == false)
        {
            timeStepAvailablityBuffer[1] = true;
        }else if(timeStepAvailablityBuffer[2] == false)
        {
            timeStepAvailablityBuffer[2] = true;
        }
    }

    return t_timeStep;
}

//int Grid::checkTimeStep(double t_timeStep)
//{
//    int i,j,k = 0;
//    Velocity t_v0, t_v1; //v0表示加速前速度，v1表示加速后速度
//    vec3 t_acceleration;
//    vec3 shifting;


//    //Step 1:在当前步长尝试运算下一状态
//    for(i = 0; i < N; i++)
//    {
//        for(j = 0; j < N; j++)
//        {

//            if(i==0&&j==0)
//            {
//                int ttt =1;
//            }
//            t_v0 = *arr_mass_TimeStepPrediction[i][j]->m_velocity;

//            t_acceleration = arr_mass_TimeStepPrediction[i][j]->m_force->m_force / arr_mass_TimeStepPrediction[i][j]->m_mass;//F = ma计算得出加速度

//            t_v1 = t_v0 + t_acceleration * t_timeStep;

//            shifting = t_v1.m_velocity * t_timeStep;
//            if(!arr_mass_TimeStepPrediction[i][j]->stabled){
//                arr_mass_TimeStepPrediction[i][j]->m_velocity->m_velocity = t_v1.m_velocity;
//                arr_mass_TimeStepPrediction[i][j]->m_position = arr_mass_TimeStepPrediction[i][j]->m_position+shifting;
//            }

//            //std::cout<<arr_mass[i][j]->m_position<<std::endl;

//        }
//    }

//    int result;
//    //Step 2:根据新的Mass状态，判断步长是否合理
//    for(i = 0; i < N; i++)
//    {
//        for(j = 0; j < N; j++)
//        {
//            result = arr_mass_TimeStepPrediction[i][j]->checkIfShiftOutOfRange();
//            if(result==0||result==1)
//            {
//                //恢复预测数组的状态
//                updateTimeStepPredictionMassArray();

//                //返回0，说明当前的步长仍然不合适
//                return result;
//            }
//        }
//    }

//    //通过测试，当前步长合适
//    return 2;

//}

//对超弹弹簧进行处理
void Grid::dynamicInverse()
{
    int i,j,k = 0;
    Velocity t_v0, t_v1; //v0表示加速前速度，v1表示加速后速度
    vec3 t_acceleration;
    vec3 shifting;
    double t_timeStep = m_timeStep;

    //Step 1:在当前步长尝试运算下一状态
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            t_v0 = *arr_mass_TimeStepPrediction[i][j]->m_velocity;

            t_acceleration = arr_mass_TimeStepPrediction[i][j]->m_force->m_force / arr_mass_TimeStepPrediction[i][j]->m_mass;//F = ma计算得出加速度

            t_v1 = t_v0 + t_acceleration * t_timeStep;

            shifting = t_v1.m_velocity * t_timeStep;
            if(!arr_mass_TimeStepPrediction[i][j]->stabled){
                arr_mass_TimeStepPrediction[i][j]->m_velocity->m_velocity = t_v1.m_velocity;
                arr_mass_TimeStepPrediction[i][j]->m_position = arr_mass_TimeStepPrediction[i][j]->m_position+shifting;
            }
        }
    }


    double stiffness;
    double shift;
    double origin_length;
    Spring* spring;
    Mass* n_mass1=0;
    Mass* n_mass2=0;

    //Step 2:根据新的Mass状态，判断是否发生超弹
    for(i = 0; i < N && n_mass1==0; i++)
    {
        for(j = 0; j < N && n_mass1==0; j++)
        {
            for(int k=0;k<=7 && n_mass1==0;k++)
            {
                spring = arr_mass_TimeStepPrediction[i][j]->arr_spring[k];
                if(spring !=0 ){
                    shift = spring->get_current_length() - spring->m_original_length;
                    stiffness = spring->m_range;
                    shift = shift > 0 ? shift : -shift;
                    if(shift >= spring->m_original_length * stiffness){
                        n_mass1 = spring->m_mass1;
                        n_mass2 = spring->m_mass2;
                        origin_length = spring->m_original_length;
                    }
                }
            }
        }
    }

    vec3 direction;
    double p = 0.8;//回弹后为原来长度的百分之80
    double length = n_mass1->m_position.distanceFrom(n_mass2->m_position);
    double length1= length * p;
    double factor = (1-p)/2;
    if( origin_length <= length)//伸张状态
    {
        direction = (n_mass2->m_position - n_mass1->m_position).standarize();
    }
    else if( origin_length >= length)//收缩状态
    {
        direction = (n_mass1->m_position - n_mass2->m_position).standarize();
    }

    //实现回弹过程
    if(n_mass1->stabled == false && n_mass2->stabled == false)
    {
        //处理n_mass1
        n_mass1->m_position = n_mass1->m_position + direction * length * factor;
        //处理n_mass2
        n_mass2->m_position = n_mass2->m_position + vec3{-direction.x,-direction.y,-direction.z} * length * factor;
    }else if(n_mass1->stabled == true && n_mass2->stabled == false)
    {
        //处理n_mass2
        n_mass2->m_position = n_mass2->m_position + vec3{-direction.x,-direction.y,-direction.z} * length * factor * 2;
    }else if(n_mass1->stabled == false && n_mass2->stabled == true)
    {
        //处理n_mass1
        n_mass1->m_position = n_mass1->m_position + direction * length * factor;
    }
}


bool Grid::checkTimeStep(double t_timeStep)
{
    int i,j,k = 0;
    Velocity t_v0, t_v1; //v0表示加速前速度，v1表示加速后速度
    vec3 t_acceleration;
    vec3 shifting;


    //Step 1:在当前步长尝试运算下一状态
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {

            if(i==0&&j==0)
            {
                int ttt =1;
            }
            t_v0 = *arr_mass_TimeStepPrediction[i][j]->m_velocity;

            t_acceleration = arr_mass_TimeStepPrediction[i][j]->m_force->m_force / arr_mass_TimeStepPrediction[i][j]->m_mass;//F = ma计算得出加速度

            t_v1 = t_v0 + t_acceleration * t_timeStep;

            shifting = t_v1.m_velocity * t_timeStep;
            if(!arr_mass_TimeStepPrediction[i][j]->stabled){
                arr_mass_TimeStepPrediction[i][j]->m_velocity->m_velocity = t_v1.m_velocity;
                arr_mass_TimeStepPrediction[i][j]->m_position = arr_mass_TimeStepPrediction[i][j]->m_position+shifting;
            }

            //std::cout<<arr_mass[i][j]->m_position<<std::endl;

        }
    }


    //Step 2:根据新的Mass状态，判断步长是否合理
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            if(i==0&&j==0)
            {
                int ttt =1;}
            if(arr_mass_TimeStepPrediction[i][j]->checkIfShiftOutOfRange())
            {
                //恢复预测数组的状态
                updateTimeStepPredictionMassArray();

                //返回false，说明当前的步长仍然不合适
                return false;
            }
        }
    }

    //通过测试，当前步长合适
    return true;

}

//拷贝数据，使预测数组与原始数组数据一致
void Grid::updateTimeStepPredictionMassArray()
{
    int i,j,k;
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            arr_mass_TimeStepPrediction[i][j]->m_force->m_force = arr_mass[i][j]->m_force->m_force;
            //arr_mass_TimeStepPrediction[i][j]->m_internal_force->m_force = arr_mass[i][j]->m_internal_force->m_force;
            arr_mass_TimeStepPrediction[i][j]->m_external_force->m_force = arr_mass[i][j]->m_external_force->m_force;

            arr_mass_TimeStepPrediction[i][j]->m_velocity->m_velocity = arr_mass[i][j]->m_velocity->m_velocity;
            arr_mass_TimeStepPrediction[i][j]->m_position = arr_mass[i][j]->m_position;
            arr_mass_TimeStepPrediction[i][j]->stabled = arr_mass[i][j]->stabled;

            for(k = 0;k<=11;k++)
            {
                arr_mass_TimeStepPrediction[i][j]->arr_previous_length[k] = arr_mass[i][j]->arr_previous_length[k];
            }
        }
    }
}

//Constuctor
Grid::Grid()
{
    int i,j = 0;
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N ; j++)
        {
            arr_mass[i][j] = new Mass::Mass(1);

        }
    }

    m_timeStep = 0.0001;
}

Grid::Grid(int n_N, double mass,double timestep, Force externalForce)
{
    int i,j = 0;
    N = n_N;
    m_width = n_N;
    m_height = n_N;
    arr_mass = new Mass**[N];
    arr_mass_TimeStepPrediction = new Mass**[N];
    //Step 1:初始化所有质点
    for(i = 0; i < N; i++)
    {
        arr_mass[i] = new Mass*[N];
        arr_mass_TimeStepPrediction[i] = new Mass*[N];

        for(j = 0; j < N ; j++)
        {
            //坐标系与OpenGL一致，采用右手系
            Mass* n_mass_1 = new Mass::Mass(mass,externalForce,new Velocity(vec3{0,0,0}),vec3{(double)i,(double)-j,0});
            Mass* n_mass_2 = new Mass::Mass(mass,externalForce,new Velocity(vec3{0,0,0}),vec3{(double)i,(double)-j,0});

            arr_mass[i][j]  = n_mass_1;
            arr_mass_TimeStepPrediction[i][j] = n_mass_2;
        }
    }


    //Step 2:初始化模型Mass所有的Spring
    for (i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            Mass* n_mass = arr_mass[i][j];

            std::cout<<i<<" "<<j<<std::endl;

            //Initiate the Spring array
            //[0...3] are structural spring
            //[4...7] are shear spring
            //[8...12]are flexion spring

            //Notice//
            //Position 的坐标表示与数组不同！！！
            if (n_mass->m_position == vec3{0,0,0}) {
                n_mass->arr_spring[0] = new Spring(arr_mass[0][0],arr_mass[1][0]);
                n_mass->arr_spring[1] = new Spring(arr_mass[0][0],arr_mass[0][1]);
                n_mass->arr_spring[5] = new Spring(arr_mass[0][0],arr_mass[1][1]);
                n_mass->arr_spring[8] = new Spring(arr_mass[0][0],arr_mass[2][0]);
                n_mass->arr_spring[9] = new Spring(arr_mass[0][0],arr_mass[0][2]);
            } else if (n_mass->m_position == vec3{(double)N-1,0,0}) {
                n_mass->arr_spring[1] = new Spring(arr_mass[N-1][0],arr_mass[N-1][1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[N-1][0],arr_mass[N-2][0]);
                n_mass->arr_spring[6] = new Spring(arr_mass[N-1][0],arr_mass[N-2][1]);
                n_mass->arr_spring[9] = new Spring(arr_mass[N-1][0],arr_mass[N-1][2]);
                n_mass->arr_spring[10] = new Spring(arr_mass[N-1][0],arr_mass[N-3][0]);
            } else if (n_mass->m_position == vec3{(double)N-1,-(double)(N-1),0}) {
                n_mass->arr_spring[2] = new Spring(arr_mass[N-1][N-1],arr_mass[N-2][N-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[N-1][N-1],arr_mass[N-1][N-2]);
                n_mass->arr_spring[7] = new Spring(arr_mass[N-1][N-1],arr_mass[N-2][N-2]);
                n_mass->arr_spring[10] = new Spring(arr_mass[N-1][N-1],arr_mass[N-3][N-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass[N-1][N-1],arr_mass[N-1][N-3]);
            } else if (n_mass->m_position == vec3{0,-(double)(N-1),0}) {
                n_mass->arr_spring[0] = new Spring(arr_mass[0][N-1],arr_mass[1][N-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[0][N-1],arr_mass[0][N-2]);
                n_mass->arr_spring[4] = new Spring(arr_mass[0][N-1],arr_mass[1][N-2]);
                n_mass->arr_spring[8] = new Spring(arr_mass[0][N-1],arr_mass[2][N-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass[0][N-1],arr_mass[0][N-3]);
            } else if (n_mass->m_position.x == 0) { //左边
                n_mass->arr_spring[0] = new Spring(arr_mass[0][j],arr_mass[1][j]);
                n_mass->arr_spring[1] = new Spring(arr_mass[0][j],arr_mass[0][j+1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[0][j],arr_mass[0][j-1]);
                n_mass->arr_spring[4] = new Spring(arr_mass[0][j],arr_mass[1][j-1]);
                n_mass->arr_spring[5] = new Spring(arr_mass[0][j],arr_mass[1][j+1]);
                n_mass->arr_spring[8] = new Spring(arr_mass[0][j],arr_mass[2][j]);
                if(j<=N-3)
                    n_mass->arr_spring[9] = new Spring(arr_mass[0][j],arr_mass[0][j+2]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass[0][j],arr_mass[0][j-2]);
            } else if (n_mass->m_position.y == 0) { //上边
                n_mass->arr_spring[0] = new Spring(arr_mass[i][0],arr_mass[i+1][0]);
                n_mass->arr_spring[1] = new Spring(arr_mass[i][0],arr_mass[i][1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[i][0],arr_mass[i-1][0]);
                n_mass->arr_spring[5] = new Spring(arr_mass[i][0],arr_mass[i+1][1]);
                n_mass->arr_spring[6] = new Spring(arr_mass[i][0],arr_mass[i-1][1]);
                if(i<=N-3)
                    n_mass->arr_spring[8] = new Spring(arr_mass[i][0],arr_mass[i+2][0]);
                n_mass->arr_spring[9] = new Spring(arr_mass[i][0],arr_mass[i][2]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass[i][0],arr_mass[i-2][0]);
            } else if (n_mass->m_position.x == N-1) { //右边
                n_mass->arr_spring[1] = new Spring(arr_mass[N-1][j],arr_mass[N-1][j+1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[N-1][j],arr_mass[N-2][j]);
                n_mass->arr_spring[3] = new Spring(arr_mass[N-1][j],arr_mass[N-1][j-1]);
                n_mass->arr_spring[6] = new Spring(arr_mass[N-1][j],arr_mass[N-2][j+1]);
                n_mass->arr_spring[7] = new Spring(arr_mass[N-1][j],arr_mass[N-2][j-1]);
                if(j<=N-3)
                    n_mass->arr_spring[9] = new Spring(arr_mass[N-1][j],arr_mass[N-1][j+2]);
                n_mass->arr_spring[10] = new Spring(arr_mass[N-1][j],arr_mass[N-3][j]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass[N-1][j],arr_mass[N-1][j-2]);
            } else if (n_mass->m_position.y == -(N-1)) { //下边
                n_mass->arr_spring[0] = new Spring(arr_mass[i][N-1],arr_mass[i+1][N-1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[i][N-1],arr_mass[i-1][N-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[i][N-1],arr_mass[i][N-2]);
                n_mass->arr_spring[4] = new Spring(arr_mass[i][N-1],arr_mass[i+1][N-2]);
                n_mass->arr_spring[7] = new Spring(arr_mass[i][N-1],arr_mass[i-1][N-2]);
                if(i<=N-3)
                    n_mass->arr_spring[8] = new Spring(arr_mass[i][N-1],arr_mass[i+2][N-1]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass[i][N-1],arr_mass[i-2][N-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass[i][N-1],arr_mass[i][N-3]);
            } else {

                n_mass->arr_spring[0] = new Spring(arr_mass[i][j],arr_mass[i+1][j]);
                n_mass->arr_spring[1] = new Spring(arr_mass[i][j],arr_mass[i][j+1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[i][j],arr_mass[i-1][j]);
                n_mass->arr_spring[3] = new Spring(arr_mass[i][j],arr_mass[i][j-1]);

                n_mass->arr_spring[4] = new Spring(arr_mass[i][j],arr_mass[i+1][j-1]);
                n_mass->arr_spring[5] = new Spring(arr_mass[i][j],arr_mass[i+1][j+1]);
                n_mass->arr_spring[6] = new Spring(arr_mass[i][j],arr_mass[i-1][j+1]);
                n_mass->arr_spring[7] = new Spring(arr_mass[i][j],arr_mass[i-1][j-1]);

                if(i<=N-3)
                    n_mass->arr_spring[8]  = new Spring(arr_mass[i][j],arr_mass[i+2][j]);
                if(j<=N-3)
                    n_mass->arr_spring[9]  = new Spring(arr_mass[i][j],arr_mass[i][j+2]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass[i][j],arr_mass[i-2][j]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass[i][j],arr_mass[i][j-2]);
            }

        }
    }

    //Step 3:初始化预测Mass所有的Spring
    for (i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            Mass* n_mass = arr_mass_TimeStepPrediction[i][j];

            std::cout<<i<<" "<<j<<std::endl;

            //Initiate the Spring array
            //[0...3] are structural spring
            //[4...7] are shear spring
            //[8...12]are flexion spring

            //Notice//
            //Position 的坐标表示与数组不同！！！
            if (n_mass->m_position == vec3{0,0,0}) {
                n_mass->arr_spring[0] = new Spring(arr_mass_TimeStepPrediction[0][0],arr_mass_TimeStepPrediction[1][0]);
                n_mass->arr_spring[1] = new Spring(arr_mass_TimeStepPrediction[0][0],arr_mass_TimeStepPrediction[0][1]);
                n_mass->arr_spring[5] = new Spring(arr_mass_TimeStepPrediction[0][0],arr_mass_TimeStepPrediction[1][1]);
                n_mass->arr_spring[8] = new Spring(arr_mass_TimeStepPrediction[0][0],arr_mass_TimeStepPrediction[2][0]);
                n_mass->arr_spring[9] = new Spring(arr_mass_TimeStepPrediction[0][0],arr_mass_TimeStepPrediction[0][2]);
            } else if (n_mass->m_position == vec3{(double)N-1,0,0}) {
                n_mass->arr_spring[1] = new Spring(arr_mass_TimeStepPrediction[N-1][0],arr_mass_TimeStepPrediction[N-1][1]);
                n_mass->arr_spring[2] = new Spring(arr_mass_TimeStepPrediction[N-1][0],arr_mass_TimeStepPrediction[N-2][0]);
                n_mass->arr_spring[6] = new Spring(arr_mass_TimeStepPrediction[N-1][0],arr_mass_TimeStepPrediction[N-2][1]);
                n_mass->arr_spring[9] = new Spring(arr_mass_TimeStepPrediction[N-1][0],arr_mass_TimeStepPrediction[N-1][2]);
                n_mass->arr_spring[10] = new Spring(arr_mass_TimeStepPrediction[N-1][0],arr_mass_TimeStepPrediction[N-3][0]);
            } else if (n_mass->m_position == vec3{(double)N-1,-(double)(N-1),0}) {
                n_mass->arr_spring[2] = new Spring(arr_mass_TimeStepPrediction[N-1][N-1],arr_mass_TimeStepPrediction[N-2][N-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass_TimeStepPrediction[N-1][N-1],arr_mass_TimeStepPrediction[N-1][N-2]);
                n_mass->arr_spring[7] = new Spring(arr_mass_TimeStepPrediction[N-1][N-1],arr_mass_TimeStepPrediction[N-2][N-2]);
                n_mass->arr_spring[10] = new Spring(arr_mass_TimeStepPrediction[N-1][N-1],arr_mass_TimeStepPrediction[N-3][N-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass_TimeStepPrediction[N-1][N-1],arr_mass_TimeStepPrediction[N-1][N-3]);
            } else if (n_mass->m_position == vec3{0,-(double)(N-1),0}) {
                n_mass->arr_spring[0] = new Spring(arr_mass_TimeStepPrediction[0][N-1],arr_mass_TimeStepPrediction[1][N-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass_TimeStepPrediction[0][N-1],arr_mass_TimeStepPrediction[0][N-2]);
                n_mass->arr_spring[4] = new Spring(arr_mass_TimeStepPrediction[0][N-1],arr_mass_TimeStepPrediction[1][N-2]);
                n_mass->arr_spring[8] = new Spring(arr_mass_TimeStepPrediction[0][N-1],arr_mass_TimeStepPrediction[2][N-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass_TimeStepPrediction[0][N-1],arr_mass_TimeStepPrediction[0][N-3]);
            } else if (n_mass->m_position.x == 0) { //左边
                n_mass->arr_spring[0] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[1][j]);
                n_mass->arr_spring[1] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[0][j+1]);
                n_mass->arr_spring[3] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[0][j-1]);
                n_mass->arr_spring[4] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[1][j-1]);
                n_mass->arr_spring[5] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[1][j+1]);
                n_mass->arr_spring[8] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[2][j]);
                if(j<=N-3)
                    n_mass->arr_spring[9] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[0][j+2]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass_TimeStepPrediction[0][j],arr_mass_TimeStepPrediction[0][j-2]);
            } else if (n_mass->m_position.y == 0) { //上边
                n_mass->arr_spring[0] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i+1][0]);
                n_mass->arr_spring[1] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i][1]);
                n_mass->arr_spring[2] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i-1][0]);
                n_mass->arr_spring[5] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i+1][1]);
                n_mass->arr_spring[6] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i-1][1]);
                if(i<=N-3)
                    n_mass->arr_spring[8] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i+2][0]);
                n_mass->arr_spring[9] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i][2]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass_TimeStepPrediction[i][0],arr_mass_TimeStepPrediction[i-2][0]);
            } else if (n_mass->m_position.x == N-1) { //右边
                n_mass->arr_spring[1] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-1][j+1]);
                n_mass->arr_spring[2] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-2][j]);
                n_mass->arr_spring[3] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-1][j-1]);
                n_mass->arr_spring[6] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-2][j+1]);
                n_mass->arr_spring[7] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-2][j-1]);
                if(j<=N-3)
                    n_mass->arr_spring[9] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-1][j+2]);
                n_mass->arr_spring[10] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-3][j]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass_TimeStepPrediction[N-1][j],arr_mass_TimeStepPrediction[N-1][j-2]);
            } else if (n_mass->m_position.y == -(N-1)) { //下边
                n_mass->arr_spring[0] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i+1][N-1]);
                n_mass->arr_spring[2] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i-1][N-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i][N-2]);
                n_mass->arr_spring[4] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i+1][N-2]);
                n_mass->arr_spring[7] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i-1][N-2]);
                if(i<=N-3)
                    n_mass->arr_spring[8] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i+2][N-1]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i-2][N-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass_TimeStepPrediction[i][N-1],arr_mass_TimeStepPrediction[i][N-3]);
            } else {

                n_mass->arr_spring[0] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i+1][j]);
                n_mass->arr_spring[1] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i][j+1]);
                n_mass->arr_spring[2] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i-1][j]);
                n_mass->arr_spring[3] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i][j-1]);

                n_mass->arr_spring[4] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i+1][j-1]);
                n_mass->arr_spring[5] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i+1][j+1]);
                n_mass->arr_spring[6] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i-1][j+1]);
                n_mass->arr_spring[7] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i-1][j-1]);

                if(i<=N-3)
                    n_mass->arr_spring[8]  = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i+2][j]);
                if(j<=N-3)
                    n_mass->arr_spring[9]  = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i][j+2]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i-2][j]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass_TimeStepPrediction[i][j],arr_mass_TimeStepPrediction[i][j-2]);
            }

        }
    }

    //初始化position数组
    arr_position = new GLfloat[N*N*3];

    m_timeStep = timestep;
    m_critical_timeStep = 3.1415926*sqrt(arr_mass[0][0]->m_mass/arr_mass[0][0]->arr_spring[1]->m_elasticity);
    m_externalForce = new Force(externalForce.m_force);
}

//任意矩形的Grid构造函数
Grid::Grid(int width,int height, double mass,double timestep, Force externalForce)
{
    int i,j = 0;
    m_width = width;
    m_height = height;
    arr_mass = new Mass**[height];
    //Step 1:初始化所有质点
    for(i = 0; i < height; i++)
    {
        arr_mass[i] = new Mass*[width];
        for(j = 0; j < width ; j++)
        {
            //坐标系与OpenGL一致，采用右手系
            Mass* n_mass = new Mass::Mass(mass,externalForce,new Velocity(vec3{0,0,0}),vec3{(double)i,(double)-j,0});

            arr_mass[i][j]  = n_mass;
        }
    }

    //Step 2:初始化所有Spring
    for (i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            Mass* n_mass = arr_mass[i][j];

            std::cout<<i<<" "<<j<<std::endl;

            //Initiate the Spring array
            //[0...3] are structural spring
            //[4...7] are shear spring
            //[8...12]are flexion spring

            //Notice//
            //Position 的坐标表示与数组不同！！！
            if (n_mass->m_position == vec3{0,0,0}) {
                n_mass->arr_spring[0] = new Spring(arr_mass[0][0],arr_mass[1][0]);
                n_mass->arr_spring[1] = new Spring(arr_mass[0][0],arr_mass[0][1]);
                n_mass->arr_spring[5] = new Spring(arr_mass[0][0],arr_mass[1][1]);
                n_mass->arr_spring[8] = new Spring(arr_mass[0][0],arr_mass[2][0]);
                n_mass->arr_spring[9] = new Spring(arr_mass[0][0],arr_mass[0][2]);
            } else if (n_mass->m_position == vec3{(double)width-1,0,0}) {
                n_mass->arr_spring[1] = new Spring(arr_mass[width-1][0],arr_mass[width-1][1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[width-1][0],arr_mass[width-2][0]);
                n_mass->arr_spring[6] = new Spring(arr_mass[width-1][0],arr_mass[width-2][1]);
                n_mass->arr_spring[9] = new Spring(arr_mass[width-1][0],arr_mass[width-1][2]);
                n_mass->arr_spring[10] = new Spring(arr_mass[width-1][0],arr_mass[width-3][0]);
            } else if (n_mass->m_position == vec3{(double)width-1,-(double)(height-1),0}) {
                n_mass->arr_spring[2] = new Spring(arr_mass[width-1][height-1],arr_mass[width-2][height-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[width-1][height-1],arr_mass[width-1][height-2]);
                n_mass->arr_spring[7] = new Spring(arr_mass[width-1][height-1],arr_mass[width-2][height-2]);
                n_mass->arr_spring[10] = new Spring(arr_mass[width-1][height-1],arr_mass[width-3][height-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass[width-1][height-1],arr_mass[width-1][height-3]);
            } else if (n_mass->m_position == vec3{0,-(double)(height-1),0}) {
                n_mass->arr_spring[0] = new Spring(arr_mass[0][height-1],arr_mass[1][height-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[0][height-1],arr_mass[0][height-2]);
                n_mass->arr_spring[4] = new Spring(arr_mass[0][height-1],arr_mass[1][height-2]);
                n_mass->arr_spring[8] = new Spring(arr_mass[0][height-1],arr_mass[2][height-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass[0][height-1],arr_mass[0][height-3]);
            } else if (n_mass->m_position.x == 0) { //左边
                n_mass->arr_spring[0] = new Spring(arr_mass[0][j],arr_mass[1][j]);
                n_mass->arr_spring[1] = new Spring(arr_mass[0][j],arr_mass[0][j+1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[0][j],arr_mass[0][j-1]);
                n_mass->arr_spring[4] = new Spring(arr_mass[0][j],arr_mass[1][j-1]);
                n_mass->arr_spring[5] = new Spring(arr_mass[0][j],arr_mass[1][j+1]);
                n_mass->arr_spring[8] = new Spring(arr_mass[0][j],arr_mass[2][j]);
                if(j<=N-3)
                    n_mass->arr_spring[9] = new Spring(arr_mass[0][j],arr_mass[0][j+2]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass[0][j],arr_mass[0][j-2]);
            } else if (n_mass->m_position.y == 0) { //上边
                n_mass->arr_spring[0] = new Spring(arr_mass[i][0],arr_mass[i+1][0]);
                n_mass->arr_spring[1] = new Spring(arr_mass[i][0],arr_mass[i][1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[i][0],arr_mass[i-1][0]);
                n_mass->arr_spring[5] = new Spring(arr_mass[i][0],arr_mass[i+1][1]);
                n_mass->arr_spring[6] = new Spring(arr_mass[i][0],arr_mass[i-1][1]);
                if(i<=N-3)
                    n_mass->arr_spring[8] = new Spring(arr_mass[i][0],arr_mass[i+2][0]);
                n_mass->arr_spring[9] = new Spring(arr_mass[i][0],arr_mass[i][2]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass[i][0],arr_mass[i-2][0]);
            } else if (n_mass->m_position.x == width-1) { //右边
                n_mass->arr_spring[1] = new Spring(arr_mass[width-1][j],arr_mass[width-1][j+1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[width-1][j],arr_mass[width-2][j]);
                n_mass->arr_spring[3] = new Spring(arr_mass[width-1][j],arr_mass[width-1][j-1]);
                n_mass->arr_spring[6] = new Spring(arr_mass[width-1][j],arr_mass[width-2][j+1]);
                n_mass->arr_spring[7] = new Spring(arr_mass[width-1][j],arr_mass[width-2][j-1]);
                if(j<=height-3)
                    n_mass->arr_spring[9] = new Spring(arr_mass[width-1][j],arr_mass[width-1][j+2]);
                n_mass->arr_spring[10] = new Spring(arr_mass[width-1][j],arr_mass[width-3][j]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass[width-1][j],arr_mass[width-1][j-2]);
            } else if (n_mass->m_position.y == -(height-1)) { //下边
                n_mass->arr_spring[0] = new Spring(arr_mass[i][height-1],arr_mass[i+1][height-1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[i][height-1],arr_mass[i-1][height-1]);
                n_mass->arr_spring[3] = new Spring(arr_mass[i][height-1],arr_mass[i][height-2]);
                n_mass->arr_spring[4] = new Spring(arr_mass[i][height-1],arr_mass[i+1][height-2]);
                n_mass->arr_spring[7] = new Spring(arr_mass[i][height-1],arr_mass[i-1][height-2]);
                if(i<=width-3)
                    n_mass->arr_spring[8] = new Spring(arr_mass[i][height-1],arr_mass[i+2][height-1]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass[i][height-1],arr_mass[i-2][height-1]);
                n_mass->arr_spring[11] = new Spring(arr_mass[i][height-1],arr_mass[i][height-3]);
            } else {

                n_mass->arr_spring[0] = new Spring(arr_mass[i][j],arr_mass[i+1][j]);
                n_mass->arr_spring[1] = new Spring(arr_mass[i][j],arr_mass[i][j+1]);
                n_mass->arr_spring[2] = new Spring(arr_mass[i][j],arr_mass[i-1][j]);
                n_mass->arr_spring[3] = new Spring(arr_mass[i][j],arr_mass[i][j-1]);

                n_mass->arr_spring[4] = new Spring(arr_mass[i][j],arr_mass[i+1][j-1]);
                n_mass->arr_spring[5] = new Spring(arr_mass[i][j],arr_mass[i+1][j+1]);
                n_mass->arr_spring[6] = new Spring(arr_mass[i][j],arr_mass[i-1][j+1]);
                n_mass->arr_spring[7] = new Spring(arr_mass[i][j],arr_mass[i-1][j-1]);

                if(i<=height-3)
                    n_mass->arr_spring[8]  = new Spring(arr_mass[i][j],arr_mass[i+2][j]);
                if(j<=width-3)
                    n_mass->arr_spring[9]  = new Spring(arr_mass[i][j],arr_mass[i][j+2]);
                if(i>=2)
                    n_mass->arr_spring[10] = new Spring(arr_mass[i][j],arr_mass[i-2][j]);
                if(j>=2)
                    n_mass->arr_spring[11] = new Spring(arr_mass[i][j],arr_mass[i][j-2]);
            }

        }
    }

    //初始化position数组
    arr_position = new GLfloat[width*height*3];

    m_timeStep = timestep;
    m_critical_timeStep = 3.1415926*sqrt(arr_mass[0][0]->m_mass/arr_mass[0][0]->arr_spring[1]->m_elasticity);
    m_externalForce = new Force(externalForce.m_force);

    timeStepAvailablityBuffer[0] = false;
    timeStepAvailablityBuffer[1] = false;
    timeStepAvailablityBuffer[2] = false;
}

void Grid::setElasticity(double e)
{

    for (int i=0;i<m_height;i++)
    {
        for(int j=0;j<m_width;j++)
        {
            Mass* n_mass = arr_mass[i][j];

            for(int k=0;k<12;k++)
            {
                if(n_mass->arr_spring[k] != 0)
                {
                    n_mass->arr_spring[k]->m_elasticity = e;
                }
            }
        }
    }
}

void Grid::setRange(double r)
{
    for (int i=0;i<m_height;i++)
    {
        for(int j=0;j<m_width;j++)
        {
            Mass* n_mass = arr_mass[i][j];

            for(int k=0;k<12;k++)
            {
                if(n_mass->arr_spring[k] != 0)
                {
                    n_mass->arr_spring[k]->m_range = r;
                }
            }
        }
    }
}
