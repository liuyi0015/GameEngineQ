//
// Created by XL0002 on 2026/8/5.
//

#ifndef GAMEENGINE_HELLOWORLDSYSTEM_H
#define GAMEENGINE_HELLOWORLDSYSTEM_H
#include <memory>
#include "../../ecs/Util.h"

class HelloWorldSystem:public ecs::System{

public:
    HelloWorldSystem(ecs::Scene* scene) :System(scene){
    }
    void start() override {
        std::cout<<"Hello, World!"<<std::endl;
        std::cout<<"Hello, 3D!"<<std::endl;
    };
    void update(double deltaTime) override{} ;
    void fixed_update(double deltaTime) override{};
    void draw() override{};
};

#endif //GAMEENGINE_HELLOWORLDSYSTEM_H
