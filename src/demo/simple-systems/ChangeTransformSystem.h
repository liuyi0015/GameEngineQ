//
// Created by XL0002 on 2026/8/3.
//

#ifndef GAMEENGINE_ROTATIONSYSTEM_H
#define GAMEENGINE_ROTATIONSYSTEM_H
#include "../GameComponents.h"
#include "../../ecs/MonoBehaviourSystem.h"


class ChangeTransformSystem : public ecs::MonoBehaviourSystem<RotationFlag> {
public:
    explicit ChangeTransformSystem(ecs::Scene* scene)
        : ecs::MonoBehaviourSystem<RotationFlag>(scene) {

    }

    void onStart() override{};
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override;

    void rotate(double deltaTime);

    void scale(double deltaTime);

    void move(double deltaTime);

    void onDraw() const override{};
};


#endif //GAMEENGINE_ROTATIONSYSTEM_H
