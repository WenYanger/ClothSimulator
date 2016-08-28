//
//  DefaultTypes.hpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/11.
//  Copyright © 2016年 WenYang. All rights reserved.
//

#ifndef DefaultTypes_hpp
#define DefaultTypes_hpp

#include <iostream>
#include <math.h>
#include <QtGui>
#include <QtOpenGL>
#include <glu.h>

using namespace std;

//Represent a vector in 3 Dimensions
struct vec3
{
    double x;
    double y;
    double z;


    //标准化向量，使值变为1
    vec3 standarize()
    {
        double factor = sqrt(x*x + y*y + z*z);
        return vec3{ x/factor, y/factor, z/factor };
    }

    //计算长度
    double length()
    {
        return sqrt(x*x + y*y + z*z);
    }

    double distanceFrom(vec3 & Vector)
    {
        double dX = x - Vector.x;
        double dY = y - Vector.y;
        double dZ = z - Vector.z;

        return sqrt(dX*dX + dY*dY + dZ*dZ);
    }

    friend ostream &operator<<(ostream &out,vec3 Vector);

    vec3 operator+(const vec3 & Vector)
    {
        return vec3
        {
            x + Vector.x,
            y + Vector.y,
            z + Vector.z
        };
    }

    vec3 operator-(const vec3 & Vector)
    {
        return vec3
        {
            x - Vector.x,
            y - Vector.y,
            z - Vector.z
        };
    }

    vec3 operator*(double factor)
    {
        return vec3
        {
            x * factor,
            y * factor,
            z * factor
        };
    }

    vec3 operator/(double factor)
    {
        return vec3
        {
            x / factor,
            y / factor,
            z / factor
        };
    }

    bool operator==(const vec3 & Vector)
    {
        return x == Vector.x && y == Vector.y && z == Vector.z;
    }

};




#endif /* DefaultTypes_hpp */
