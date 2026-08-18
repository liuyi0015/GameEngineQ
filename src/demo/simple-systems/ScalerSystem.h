//
// Created by XL0002 on 2026/7/28.
//

#ifndef GAMEENGINE_SCALERSYSTEM_H
#define GAMEENGINE_SCALERSYSTEM_H
#include "../GameComponents.h"
#include "../../ecs/MonoBehaviourSystem.h"


class ScalerSystem :public ecs::MonoBehaviourSystem<ScalerFlag>{
    public:
    explicit ScalerSystem(ecs::Scene* scene)
        : ecs::MonoBehaviourSystem<ScalerFlag>(scene) {

    }

    void onStart() override{};
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override;
    void onDraw() const override{};
};


#endif //GAMEENGINE_SCALERSYSTEM_H
