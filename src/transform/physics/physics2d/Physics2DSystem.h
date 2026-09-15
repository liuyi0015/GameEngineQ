//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_PHYSICS2DSYSTEM_H
#define GAMEENGINE_PHYSICS2DSYSTEM_H

#include <unordered_map>
#include <vector>

#include "../../../core/ecs/System.h"
#include "box2d/id.h"
#include "../../../core/ecs/Scene.h"
#include "box2d/types.h"

class Physics2DSystem:public ecs::System{
private:
    b2WorldId worldId{};
    std::unordered_map<Entity,b2BodyId> bodyIds;
public:
    explicit Physics2DSystem(ecs::Scene* scene)
        : System(scene){
    }
    ~Physics2DSystem()override;
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override;
    void draw() override{};
    b2BodyId getBodyIdsByEntity(Entity entity);
    void setVelocity(Entity entity,b2Vec2 vel);
};


#endif //GAMEENGINE_PHYSICS2DSYSTEM_H
