//
// Created by XL0002 on 2026/7/28.
//

#ifndef GAMEENGINE_PREFABS_H
#define GAMEENGINE_PREFABS_H

#include "../ecs/Scene.h"
class Prefabs {
public:
    static Entity physicsCircle(const std::shared_ptr<Scene> &scene);
    static Entity staticImage( const std::shared_ptr<Scene> &scene);
    static Entity button(const std::shared_ptr<Scene> &scene,std::optional<Entity> parent);
    static Entity anim1(const std::shared_ptr<Scene> &scene);
    static Entity camera(const std::shared_ptr<Scene> &scene);
};


#endif //GAMEENGINE_PREFABS_H
