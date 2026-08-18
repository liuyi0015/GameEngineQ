//
// Created by XL0002 on 2026/7/15.
//

#ifndef OPENVISUALNOVEL_SYSTEM_H
#define OPENVISUALNOVEL_SYSTEM_H


namespace ecs {
    class Scene;

    class System {
    public:
        //这里只是用指针成员代替传参，普通system是scene的成员，全局system与scene独立，反正都不是包含scene
        Scene* scene;
        // System()=delete;
        explicit System(Scene* scene) :
            scene(scene) {
        }
        virtual ~System() = default;
        virtual void start(){};
        virtual void update(double deltaTime){};
        virtual void fixed_update(double deltaTime){};
        virtual void draw(){};
    };

}


#endif //OPENVISUALNOVEL_SYSTEM_H
