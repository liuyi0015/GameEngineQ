//
// Created by XL0002 on 2026/7/21.
//

#include "Physics2DSystem.h"

#include <cassert>

#include "../Context.hpp"
#include "box2d/box2d.h"
#include "../ecs/BaseComponents.h"
#include "../Config.h"
void Physics2DSystem::start() {

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -10.0f};
    worldId = b2CreateWorld(&worldDef);

    for (const auto entity:scene->getEntities()) {
        auto enabledComp = scene->getComponent<ecs::Enabled>(entity);
        auto rigidBodyComp = scene->getComponent<RigidBodyComp>(entity);
        if (!enabledComp.has_value()||!enabledComp.value().value || !rigidBodyComp.has_value()) {
            continue;
        }
        auto transformComp = scene->getComponent<ecs::Transform>(entity);
        assert(transformComp.has_value());
        b2BodyDef bodyDef=b2DefaultBodyDef();
        bodyDef.type=rigidBodyComp.value().type;

        bodyDef.position=(b2Vec2){transformComp.value().position.x,transformComp.value().position.y};
        const b2BodyId bodyId=b2CreateBody(worldId,&bodyDef);
        bodyIds[entity] = bodyId;
    }

}

void Physics2DSystem::fixed_update(double deltaTime) {
    auto config= ApplicationContext::getInstance().get<Config>("config");
    b2World_Step(worldId,static_cast<float>(deltaTime), 4);
    for (const auto entity:scene->getEntities()) {
        auto enabledComp = scene->getComponent<ecs::Enabled>(entity);
        auto rigidBodyComp = scene->getComponent<RigidBodyComp>(entity);
        if (!enabledComp.has_value()||!enabledComp.value().value || !rigidBodyComp.has_value()) {
            continue;
        }
        b2BodyId bodyId = bodyIds[entity];
        b2Vec2 position = b2Body_GetPosition(bodyId);
        b2Rot rotation = b2Body_GetRotation(bodyId);
        auto transformComp = scene->getComponent<ecs::Transform>(entity);
        assert(transformComp.has_value()) ;
        transformComp.value().position = {position.x, position.y};
        transformComp.value().rotation.angle = atan2f(rotation.s, rotation.c);
        scene->setComponent<ecs::Transform>(entity, transformComp.value());
        // std::cout<<"physics"<<std::endl;
    }
}

