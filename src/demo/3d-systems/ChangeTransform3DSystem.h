//
// Created by XL0002 on 2026/9/21.
//

#ifndef GAMEENGINEQ_CHANGETRANSFORM3DSYSTEM_H
#define GAMEENGINEQ_CHANGETRANSFORM3DSYSTEM_H
#include "../../ecs/MonoBehaviourSystem.h"
struct Rotation3DFlag {
    float speedX;
    float speedY;
    float speedZ;
};
class ChangeTransform3DSystem :public ecs::MonoBehaviourSystem<Rotation3DFlag>{

public:
    explicit ChangeTransform3DSystem(ecs::Scene* scene)
        : ecs::MonoBehaviourSystem<Rotation3DFlag>(scene) {

    }

    void onStart() override{};
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override;
    void onDraw() const override{};
};


#endif //GAMEENGINEQ_CHANGETRANSFORM3DSYSTEM_H
