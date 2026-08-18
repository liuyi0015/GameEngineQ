//
// Created by XL0002 on 2026/7/28.
//

#ifndef GAMEENGINE_PREFABS_H
#define GAMEENGINE_PREFABS_H

#include <optional>

#include "../ecs/Scene.h"
class Prefabs {
public:
    static Entity physicsCircle(ecs::Scene* scene);

    static Entity staticGround(ecs::Scene* scene);

    static Entity staticImage( ecs::Scene* scene);
    static Entity button(ecs::Scene* scene,std::optional<Entity> parent);

    static Entity cube(ecs::Scene *scene);

    static Entity anim1(ecs::Scene* scene, std::optional<Entity> parent);
    static Entity camera(ecs::Scene* scene);
};


#endif //GAMEENGINE_PREFABS_H
