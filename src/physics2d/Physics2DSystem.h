//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_PHYSICS2DSYSTEM_H
#define GAMEENGINE_PHYSICS2DSYSTEM_H

#include <unordered_map>
#include <vector>

#include "../ecs/System.h"
#include "box2d/id.h"
#include "../ecs/Scene.h"
#include "box2d/types.h"

struct RigidBodyComp {
    b2BodyType type;
};
class Physics2DSystem:public ecs::System{
private:
    b2WorldId worldId{};
    std::unordered_map<Entity,b2BodyId> bodyIds;
public:
    explicit Physics2DSystem(const std::shared_ptr<Scene> &scene)
        : System(scene){
    }
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override;
    void draw() override{};
};


#endif //GAMEENGINE_PHYSICS2DSYSTEM_H
