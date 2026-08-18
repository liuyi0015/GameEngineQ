//
// Created by XL0002 on 2026/8/10.
//

#ifndef GAMEENGINE_JUMPSYSTEM_H
#define GAMEENGINE_JUMPSYSTEM_H

#include "../../ecs/MonoBehaviourSystem.h"
#include "../GameComponents.h"

class JumpSystem :public ecs::MonoBehaviourSystem<JumpFlag>{
private:
    SystemId bindPhysicSystem;
public:
    explicit JumpSystem(ecs::Scene* scene,SystemId bindPhysicSystem)
        : ecs::MonoBehaviourSystem<JumpFlag>(scene) , bindPhysicSystem(bindPhysicSystem){
    }
    ~JumpSystem()override;
    void onStart() override;
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override{};
    void onDraw() const override{};
};


#endif //GAMEENGINE_JUMPSYSTEM_H
