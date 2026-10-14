//
// Created by XL0002 on 2026/8/6.
//

#ifndef GAMEENGINE_TRANSFORM3DCOMPONENTS_H
#define GAMEENGINE_TRANSFORM3DCOMPONENTS_H
#include <optional>
#include "glm/vec3.hpp"
#include "../../ecs/Entity.h"

struct Transform3D {
    glm::vec3 position={0,0,0};
    glm::vec3 rotation={0,0,0};
    glm::vec3 scale={1,1,1};
};
#endif //GAMEENGINE_TRANSFORM3DCOMPONENTS_H
