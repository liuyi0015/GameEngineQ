//
// Created by XL0002 on 2026/9/20.
//

#ifndef GAMEENGINEQ_TRANSFORMUTIL_H
#define GAMEENGINEQ_TRANSFORMUTIL_H

#include <optional>
#include "../../transform/transform2d/Transform2DUtil.h"
#include "../../transform/transform3d/Transform3DUtil.h"

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
    static Transform3D computeWorldToLocalTransform(const Transform3D &worldTransform,
        const Transform3D &parentWorldTransform);

    static Transform2D computeLocalToWorldTransform(const Transform2DComp &localTransform,
                                                  ecs::Scene* scene);
    static Transform3D computeLocalToWorldTransform(const Transform3DComp &localTransformComp,
                                                  ecs::Scene* scene);
    static Transform2D computeRelativeTransform(const Transform2DComp &from,
        const Transform2DComp &to, ecs::Scene* scene);
    static Transform3D computeRelativeTransform(const Transform3DComp &from,
        const Transform3DComp &to, ecs::Scene* scene);
    static std::vector<Entity> getChildEntities(
        ecs::Scene *scene, Entity e1);
};


#endif //GAMEENGINEQ_TRANSFORMUTIL_H
