//
// Created by XL0002 on 2026/7/16.
//

#ifndef GAMEENGINE_MOVESYSTEM_H
#define GAMEENGINE_MOVESYSTEM_H
#include <memory>

#include "../GameComponents.h"
#include "../../ecs/MonoBehaviourSystem.h"
#include "../../ecs/Scene.h"


class MoveSystem : public ecs::MonoBehaviourSystem<MoveFlag> {
public:
    explicit MoveSystem(ecs::Scene* scene)
        : ecs::MonoBehaviourSystem<MoveFlag>(scene) {
        
    }

    void onStart() override{};
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override;
    void onDraw() const override{};
};



#endif //GAMEENGINE_MOVESYSTEM_H
