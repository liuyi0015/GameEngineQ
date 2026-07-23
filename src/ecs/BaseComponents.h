//
// Created by XL0002 on 2026/7/17.
//
#pragma once

#ifndef GAMEENGINE_BASECOMPONENTS_H
#define GAMEENGINE_BASECOMPONENTS_H
#include "Scene.h"

#endif //GAMEENGINE_BASECOMPONENTS_H
namespace ecs {

    struct Enabled {
        bool value;
    };

    struct Position {
        float x = 0.0f;
        float y = 0.0f;
    };
    struct Rotation {
        float angle=0.0f;
    };
    struct Scale {
        float x = 1.0f;
        float y = 1.0f;
    };
    struct Transform {
        Position position;
        Rotation rotation;
        Scale scale;
        //父子物体只有变换关系，即有了Transform才有父子物体的概念
        std::optional<Entity> parent;
    };
}
