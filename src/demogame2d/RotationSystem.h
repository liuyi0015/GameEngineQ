//
// Created by XL0002 on 2026/8/3.
//

#ifndef GAMEENGINE_ROTATIONSYSTEM_H
#define GAMEENGINE_ROTATIONSYSTEM_H
#include "GameComponents.h"
#include <memory>
#include "../ecs/MonoBehaviourSystem.h"
#include "../ecs/Scene.h"


class RotationSystem : public ecs::MonoBehaviourSystem<RotationFlag> {
public:
    explicit RotationSystem(const std::shared_ptr<Scene>& scene)
        : ecs::MonoBehaviourSystem<RotationFlag>(scene) {

    }

    void onStart() override{};
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override;
    void onDraw() const override{};
};


#endif //GAMEENGINE_ROTATIONSYSTEM_H
