//
// Created by XL0002 on 2026/8/12.
//

#ifndef GAMEENGINE_PHYSICS3DSYSTEM_H
#define GAMEENGINE_PHYSICS3DSYSTEM_H
#include "../../ecs/System.h"


class Physics3DSystem:public ecs::System{
private:
public:
    explicit Physics3DSystem(ecs::Scene* scene)
        : System(scene){
    }
    ~Physics3DSystem()override;
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override{};
};



#endif //GAMEENGINE_PHYSICS3DSYSTEM_H
