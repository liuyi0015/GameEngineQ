//
// Created by XL0002 on 2026/8/6.
//

#ifndef GAMEENGINE_TRANSFORM3DCOMPONENTS_H
#define GAMEENGINE_TRANSFORM3DCOMPONENTS_H
#include <optional>
#include "glm/vec3.hpp"
#include "../../core/ecs/Entity.h"

struct Transform3D {
    glm::vec3 position={0,0,0};
    glm::vec3 rotation={0,0,0};
    glm::vec3 scale={1,1,1};
};
struct Transform3DComp {
    Transform3D transform;
    //父子物体只有变换关系，即有了Transform才有父子物体的概念。如果是Transform类型，就很难知道父物体是谁
    std::optional<Entity> parent;
};
struct Camera3DComp {
};
#endif //GAMEENGINE_TRANSFORM3DCOMPONENTS_H
