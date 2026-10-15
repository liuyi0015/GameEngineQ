//
// Created by XL0002 on 2026/10/13.
//

#ifndef GAMEENGINEQ_CAMERACONTROLSYSTEM_H
#define GAMEENGINEQ_CAMERACONTROLSYSTEM_H
#include "../../ecs/Entity.h"
#include "../../ecs/System.h"
#include "SDL3/SDL_events.h"


class CameraControlSystem :public ecs::System{
private:
    size_t motionSubscribeId{};
    size_t keyboardSubscribeId{};

public:
    Entity camera{};

    void rotateCamera(const SDL_MouseMotionEvent &event);

    void forwardCamera(const SDL_KeyboardEvent &event);
    void start() override;
    void end() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override{};
};


#endif //GAMEENGINEQ_CAMERACONTROLSYSTEM_H
