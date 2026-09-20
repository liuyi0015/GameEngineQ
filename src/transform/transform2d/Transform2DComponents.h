//
// Created by XL0002 on 2026/7/29.
//

#ifndef GAMEENGINE_TRANSFORM2DCOMPONENTS_H
#define GAMEENGINE_TRANSFORM2DCOMPONENTS_H
#include "glm/vec2.hpp"

struct Transform2D {
    glm::vec2 position={0,0};
    float rotation=0;
    glm::vec2 scale={1,1};
};
#endif //GAMEENGINE_TRANSFORM2DCOMPONENTS_H
