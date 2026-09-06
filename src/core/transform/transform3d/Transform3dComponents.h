//
// Created by XL0002 on 2026/8/6.
//

#ifndef GAMEENGINE_TRANSFORM3DCOMPONENTS_H
#define GAMEENGINE_TRANSFORM3DCOMPONENTS_H
#include <optional>
#include "../../ecs/Scene.h"
struct Position3D {
    float x=0.0f;
    float y=0.0f;
    float z=0.0f;
};

struct Rotation3D {
};
struct Scale3D {
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
};
struct Transform3D {
    Position3D position;
    Rotation3D rotation;
    Scale3D scale;
};
struct Transform3DComp {
    Transform3D transform;
    //父子物体只有变换关系，即有了Transform才有父子物体的概念。如果是Transform类型，就很难知道父物体是谁
    std::optional<Entity> parent;
};
struct Camera3DComp {
};
#endif //GAMEENGINE_TRANSFORM3DCOMPONENTS_H
