//
// Created by XL0002 on 2026/7/15.
//

#ifndef OPENVISUALNOVEL_SYSTEM_H
#define OPENVISUALNOVEL_SYSTEM_H
#include <memory>


class Scene;

namespace ecs {
    class System {
    public:
        //这里只用来作为参数，普通system应该是scene的成员
        std::shared_ptr<Scene> scene;
        System()=delete;
        explicit System(std::shared_ptr<Scene> scene) :
            scene(std::move(scene)) {
        }

        virtual ~System() = default;
        virtual void start(){};
        virtual void update(double deltaTime){};
        virtual void fixed_update(double deltaTime){};
        virtual void draw(){};
    };

}


#endif //OPENVISUALNOVEL_SYSTEM_H
