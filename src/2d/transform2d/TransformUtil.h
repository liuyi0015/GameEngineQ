//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_TRANSFORMUTIL_H
#define GAMEENGINE_TRANSFORMUTIL_H
#include "Transform2dComponents.h"
#include "../../ecs/BaseComponents.h"


class TransformUtil {
    public:
    static Transform computeLocalToWorldTransform(const Transform& localTransform,const std::shared_ptr<Scene>& scene);
    static Transform computeWorldToViewTransform(const Transform &worldTransform,const std::shared_ptr<Scene> &scene,Entity camera);
    static std::vector<Entity> getChildEntities(
        Entity e1, const std::shared_ptr<Scene> &scene);
};


#endif //GAMEENGINE_TRANSFORMUTIL_H
