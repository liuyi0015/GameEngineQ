//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_TRANSFORMUTIL_H
#define GAMEENGINE_TRANSFORMUTIL_H
#include "Transform2dComponents.h"
#include "../../ecs/BaseComponents.h"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat3x3.hpp"


class TransformUtil {
    public:
    static glm::mat3 transformToMatrix(const Transform &transform);

    static Transform getReverseTransform(const Transform &transform);

    static Transform computeWorldToLocalTransform(const Transform &worldTransform,
                                                  const Transform &parentWorldTransform, const std::shared_ptr<Scene> &scene);

    static Transform computeLocalToWorldTransform(const Transform& localTransform,const std::shared_ptr<Scene>& scene);
    static Transform computeWorldToViewTransform(const Transform &worldTransform,const std::shared_ptr<Scene> &scene,Entity camera);
    static Transform computeRelativeTransform(const Transform &from,const Transform &to,const std::shared_ptr<Scene> &scene);
    static std::vector<Entity> getChildEntities(
        Entity e1, const std::shared_ptr<Scene> &scene);
};


#endif //GAMEENGINE_TRANSFORMUTIL_H
