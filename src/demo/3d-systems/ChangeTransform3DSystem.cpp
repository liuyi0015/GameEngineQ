//
// Created by XL0002 on 2026/9/21.
//

#include "ChangeTransform3DSystem.h"
#include "../../transform/transform3d/Transform3dComponents.h"

void ChangeTransform3DSystem::onFixedUpdate(double deltaTime) {
    MonoBehaviourSystem<Rotation3DFlag>::onFixedUpdate(deltaTime);
    //仅旋转
    auto* transformComp = ecs::getComponent<Transform3DComp>(scene, curEntity);
    auto* rotationComp=ecs::getComponent<Rotation3DFlag>(scene, curEntity);

    transformComp->transform.rotation.x+= rotationComp->speedX * static_cast<float>(deltaTime);
    transformComp->transform.rotation.y+= rotationComp->speedY * static_cast<float>(deltaTime);
    transformComp->transform.rotation.z+= rotationComp->speedZ * static_cast<float>(deltaTime);
}
