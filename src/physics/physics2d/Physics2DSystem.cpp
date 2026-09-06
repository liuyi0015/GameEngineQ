//
// Created by XL0002 on 2026/7/21.
//

#include "Physics2DSystem.h"

#include <cassert>

#include "PhysicsComponents.h"
#include "../../core/Context.hpp"
#include "box2d/box2d.h"
#include "../../core/ecs/BaseComponents.h"
#include "../../Config.h"
#include "../../core/transform/transform2d/Transform2dComponents.h"
#include "../../core/ecs/Util.h"

Physics2DSystem::~Physics2DSystem() {
    if (b2World_IsValid(worldId)) {
        b2DestroyWorld(worldId);
    }
    //如果后续你加了关节，要再单独销毁。
}

void Physics2DSystem::start() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -300.0f};
    worldId = b2CreateWorld(&worldDef);

    for (const auto entity:ecs::getEntities<RigidBodyComp>(scene)) {
        auto enabledComp = ecs::getComponent<ecs::Enabled>(scene,entity);
        auto rigidBodyComp = ecs::getComponent<RigidBodyComp>(scene,entity);
        if (!enabledComp.has_value()||!enabledComp.value().value || !rigidBodyComp.has_value()) {
            continue;
        }
        auto transformComp = ecs::getComponent<TransformComp>(scene,entity);
        assert(transformComp.has_value());
        b2BodyDef bodyDef=b2DefaultBodyDef();
        bodyDef.type=rigidBodyComp.value().type;
        //屏幕坐标的y轴相反
        bodyDef.position=(b2Vec2){transformComp.value().transform.position.x,-transformComp.value().transform.position.y};
        //形状
        b2ShapeDef shapeDef=b2DefaultShapeDef();
        shapeDef.density=1.0f;
        b2Polygon box=b2MakeBox(50.f,50.f);
        //绑定
        const b2BodyId bodyId=b2CreateBody(worldId,&bodyDef);
        b2CreatePolygonShape(bodyId,&shapeDef,&box);
        bodyIds[entity] = bodyId;
    }

}

void Physics2DSystem::fixed_update(double deltaTime) {
    b2World_Step(worldId,static_cast<float>(deltaTime), 4);
    for (const auto entity:ecs::getEntities<RigidBodyComp>(scene)) {
        auto enabledComp = ecs::getComponent<ecs::Enabled>(scene,entity);
        if (!enabledComp.has_value()||!enabledComp.value().value ) {
            continue;
        }
        b2BodyId bodyId = bodyIds[entity];
        b2Vec2 position = b2Body_GetPosition(bodyId);
        // std::cout<<ecs::getComponent<ecs::Name>(scene,entity).value().value<<"的物理坐标"<<position.x<<" "<<position.y<<std::endl;
        b2Rot rotation = b2Body_GetRotation(bodyId);
        auto transformComp = ecs::getComponent<TransformComp>(scene,entity);
        assert(transformComp.has_value()) ;
        //屏幕坐标的y轴相反
        transformComp.value().transform.position = {position.x, -position.y};
        transformComp.value().transform.rotation.angle = atan2f(rotation.s, rotation.c);
        ecs::setComponent<TransformComp>(scene,entity, transformComp.value());
    }
}

b2BodyId Physics2DSystem::getBodyIdsByEntity(Entity entity) {
    return bodyIds[entity];
}

void Physics2DSystem::setVelocity(Entity entity, b2Vec2 vel) {
    b2BodyId bid=bodyIds[entity];
    b2Body_SetLinearVelocity(bid, vel);
}

