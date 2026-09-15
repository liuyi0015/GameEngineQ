//
// Created by XL0002 on 2026/7/29.
//

#ifndef GAMEENGINE_TRANSFORM2DCOMPONENTS_H
#define GAMEENGINE_TRANSFORM2DCOMPONENTS_H
#include <optional>
#include "../../core/ecs/Scene.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "../../soft-render/GpuSimulator.h"

struct Transform2D {
    glm::vec2 position={0,0};
    float rotation=0;
    glm::vec2 scale={1,1};
};
struct Transform2DComp {
    Transform2D transform;
    //父子物体只有变换关系，即有了Transform才有父子物体的概念。如果是Transform类型，就很难知道父物体是谁
    std::optional<Entity> parent;
};
struct Camera2DComp {
    float captureWidth;
    float captureHeight;
    ColorBuffer* target;
};
#endif //GAMEENGINE_TRANSFORM2DCOMPONENTS_H
