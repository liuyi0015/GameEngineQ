//
// Created by XL0002 on 2026/7/28.
//

#ifndef GAMEENGINE_PREFABS_H
#define GAMEENGINE_PREFABS_H

#include <optional>

#include "../ecs/Scene.h"
class Prefabs {
public:
    static Entity physicsCircle(Scene* scene);

    static Entity staticGround(Scene* scene);

    static Entity staticImage( Scene* scene);
    static Entity button(Scene* scene,std::optional<Entity> parent);
    static Entity anim1(Scene* scene, std::optional<Entity> parent);
    static Entity camera(Scene* scene);
};


#endif //GAMEENGINE_PREFABS_H
