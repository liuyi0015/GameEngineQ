//
// Created by XL0002 on 2026/7/15.
//

#ifndef OPENVISUALNOVEL_SYSTEM_H
#define OPENVISUALNOVEL_SYSTEM_H


namespace ecs {
    class Scene;

    class System {
    public:
        //这里只是用指针成员代替传参，system与scene独立，互不包含
        Scene* scene=nullptr;
        virtual ~System() = default;
        //场景加载
        virtual void start(){};
        //场景卸载
        virtual void end(){};
        virtual void update(double deltaTime){};
        virtual void fixed_update(double deltaTime){};
        virtual void draw(){};
    };

}


#endif //OPENVISUALNOVEL_SYSTEM_H
