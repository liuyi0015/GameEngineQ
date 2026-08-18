//
// Created by XL0002 on 2026/8/10.
//

#include "JumpSystem.h"

#include "../../EventDispatcher.h"
#include "../../2d/physics2d/Physics2DSystem.h"
#include "../../2d/physics2d/PhysicsComponents.h"
#include "SDL3/SDL_log.h"
static size_t subscribeId;
JumpSystem::~JumpSystem() {
    EventDispatcher::getInstance().unsubscribe(subscribeId);
}

void JumpSystem::onStart() {
    subscribeId=EventDispatcher::getInstance().subscribe("key down space",[this](std::any param) {
        auto rigid_body_comp = ecs::getComponent<RigidBodyComp>(scene,curEntity).value();
        if (rigid_body_comp.type != b2_dynamicBody) {
            SDL_Log("非dynamic类型的刚体不能跳跃！");
            return;
        }
        auto jumpFlag=ecs::getComponent<JumpFlag>(scene,curEntity).value();
        auto physicSystem=dynamic_cast<Physics2DSystem*>(ecs::getSystemById(scene,bindPhysicSystem));
        physicSystem->setVelocity(curEntity,{0,jumpFlag.speedY});
    });
}
