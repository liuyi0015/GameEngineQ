//
// Created by abc17 on 2026/7/30.
//

#ifndef GAMEENGINE_PRINTSYSTEM_H
#define GAMEENGINE_PRINTSYSTEM_H
#include <iostream>

#include "../../ecs/System.h"


class PrintSystem:public ecs::System{
private:
    int num;
public:
    PrintSystem(Scene* scene,int num) :System(scene),num(num){
    }
    void start() override {
            std::cout<<"printSystem num:"<<num<<std::endl;
    };
    void update(double deltaTime) override{} ;
    void fixed_update(double deltaTime) override{};
    void draw() override{};
};


#endif //GAMEENGINE_PRINTSYSTEM_H
