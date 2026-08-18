//
// Created by XL0002 on 2026/8/12.
//

#include "Physics3DSystem.h"

#include <iostream>

#include "box3d/box3d.h"

Physics3DSystem::~Physics3DSystem() {
}

void Physics3DSystem::start() {
    b3BodyDef s=b3DefaultBodyDef();
    std::cout<<"physics3d loaded"<<std::endl;
}
