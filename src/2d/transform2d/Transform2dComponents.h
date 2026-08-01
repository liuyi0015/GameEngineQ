//
// Created by XL0002 on 2026/7/29.
//

#ifndef GAMEENGINE_TRANSFORM2DCOMPONENTS_H
#define GAMEENGINE_TRANSFORM2DCOMPONENTS_H
#include <optional>
#include "../../ecs/Scene.h"
struct Position {
    float x = 0.0f;
    float y = 0.0f;
};

inline Position operator- (const Position& pos1, const Position& pos2){
    Position result;
    result.x = pos1.x-pos2.x;
    result.y = pos1.y-pos2.y;
    return result;
}
struct Rotation {
    float angle=0.0f;
};
struct Scale {
    float x = 1.0f;
    float y = 1.0f;
};
inline Scale operator/(const Scale s1, const Scale s2){
    Scale result;
    result.x = s1.x/s2.x;
    result.y = s1.y/s2.y;
    return result;
};
struct Transform {
    Position position;
    Rotation rotation;
    Scale scale;
    //父子物体只有变换关系，即有了Transform才有父子物体的概念。如果是Transform类型，就很难知道父物体是谁
    std::optional<Entity> parent;
};

struct CameraComp {
    float w;
    float h;
};
#endif //GAMEENGINE_TRANSFORM2DCOMPONENTS_H
