//
// Created by XL0002 on 2026/7/17.
//

#ifndef GAMEENGINE_GAMECOMPONENTS_H
#define GAMEENGINE_GAMECOMPONENTS_H
#include <string>

struct MoveFlag {
    float x;
    float y;
    float width;
    float height;
    float speed;
    int currentEdge = 0;
};//标记组件
struct ScalerFlag {
    float speed;
};
struct RotationFlag {
    float speed;
};
struct CameraInputListenerFlag {
};
struct JumpFlag {
    float speedY=1000;
};
#endif //GAMEENGINE_GAMECOMPONENTS_H
