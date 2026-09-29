//
// Created by XL0002 on 2026/7/17.
//

#ifndef GAMEENGINE_DEMOGAMEAPPLICATION_H
#define GAMEENGINE_DEMOGAMEAPPLICATION_H
#include "../core/Application.h"
#include "../ecs/Scene.h"
class DemogameApplication:public Application{
private:
    ecs::Scene* scene=nullptr;
public:
    ~DemogameApplication()override {
        delete scene;
    };
    void init()override;
    void start()override {
        scene->start();
    };
    void fixed_update(double deltaTime)override {
        scene->fixed_update(deltaTime);
    };
    void update(double deltaTime)override {
        scene->update(deltaTime);
    };
    void draw()override {
        scene->draw();
    };
};


#endif //GAMEENGINE_DEMOGAMEAPPLICATION_H
