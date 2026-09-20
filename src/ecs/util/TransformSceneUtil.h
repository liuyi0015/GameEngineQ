//
// Created by XL0002 on 2026/9/20.
//

#ifndef GAMEENGINEQ_TRANSFORMUTIL_H
#define GAMEENGINEQ_TRANSFORMUTIL_H

#include <optional>
#include "../../transform/transform2d/Transform2DUtil.h"
#include "../Scene.h"

struct Transform2DComp {
    Transform2D transform;
    //父子物体只有变换关系，即有了Transform才有父子物体的概念。如果是Transform类型，就很难知道父物体是谁
    std::optional<Entity> parent;
};
class TransformSceneUtil {
public:
    static Transform2D computeWorldToLocalTransform(const Transform2D &worldTransform,
                                                  const Transform2D &parentWorldTransform);

    static Transform2D computeLocalToWorldTransform(const Transform2DComp &localTransform,
                                                  ecs::Scene* scene);
    static Transform2D computeRelativeTransform(const Transform2DComp &from,
        const Transform2DComp &to, ecs::Scene* scene);
    static std::vector<Entity> getChildEntities(
        ecs::Scene *scene, Entity e1);
};


#endif //GAMEENGINEQ_TRANSFORMUTIL_H
