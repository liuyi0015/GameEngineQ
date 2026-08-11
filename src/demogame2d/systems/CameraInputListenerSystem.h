//
// Created by XL0002 on 2026/7/28.
//

#ifndef GAMEENGINE_INPUTCAMERALISTENERSYSTEM_H
#define GAMEENGINE_INPUTCAMERALISTENERSYSTEM_H
#include "../GameComponents.h"
#include "../../ecs/MonoBehaviourSystem.h"


class CameraInputListenerSystem:public ecs::MonoBehaviourSystem<CameraInputListenerFlag>{
    public:
    explicit CameraInputListenerSystem(Scene* scene)
        : ecs::MonoBehaviourSystem<CameraInputListenerFlag>(scene) {

    }

    void onStart() override;
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override{};
    void onDraw() const override{};
};


#endif //GAMEENGINE_INPUTCAMERALISTENERSYSTEM_H
