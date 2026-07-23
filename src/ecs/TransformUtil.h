//
// Created by XL0002 on 2026/7/21.
//

#ifndef GAMEENGINE_TRANSFORMUTIL_H
#define GAMEENGINE_TRANSFORMUTIL_H
#include "BaseComponents.h"


class TransformUtil {
    public:
    static ecs::Transform computeWorldTransform(const ecs::Transform& localTransform,const std::shared_ptr<Scene>& scene);

};


#endif //GAMEENGINE_TRANSFORMUTIL_H
