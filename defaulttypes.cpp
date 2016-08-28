#include "defaulttypes.h"

//  DefaultTypes.cpp
//  FabricSimulator
//
//  Created by WenYang on 16/3/11.
//  Copyright © 2016年 WenYang. All rights reserved.
//


ostream& operator<<(ostream &out,vec3 Vector)
{
    out<<"x: "<<Vector.x<<" y: "<<Vector.y<<" z: "<<Vector.z<<endl;
    return out;
}

