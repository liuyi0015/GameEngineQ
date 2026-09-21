//
// Created by XL0002 on 2026/9/21.
//

#include "ChangeTransform3DSystem.h"
#include "../../transform/transform3d/Transform3dComponents.h"

void ChangeTransform3DSystem::onFixedUpdate(double deltaTime) {
    MonoBehaviourSystem<Rotation3DFlag>::onFixedUpdate(deltaTime);
    //仅旋转
    auto transform = ecs::getComponent<Transform3DComp>(scene, curEntity).value();
    auto rotation=ecs::getComponent<Rotation3DFlag>(scene, curEntity).value();

    transform.transform.rotation.x+= rotation.speedX * static_cast<float>(deltaTime);
    transform.transform.rotation.y+= rotation.speedY * static_cast<float>(deltaTime);
    transform.transform.rotation.z+= rotation.speedZ * static_cast<float>(deltaTime);
    ecs::setComponent<Transform3DComp>(scene, curEntity,transform);
}
