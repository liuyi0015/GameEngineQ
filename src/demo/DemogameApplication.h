//
// Created by XL0002 on 2026/7/17.
//

#ifndef GAMEENGINE_DEMOGAMEAPPLICATION_H
#define GAMEENGINE_DEMOGAMEAPPLICATION_H
#include "simple-systems/MoveSystem.h"
#include "simple-systems/PrintSystem.h"
#include "../EcsApplication.h"
#include "../ResourceManager.hpp"

class DemogameApplication:public EcsApplication{
private:
    ecs::Scene* loadScene1() ;
    void unloadScene1() ;
    ecs::Scene* loadScene2();
    void unloadScene2();
    ecs::Scene* loadSceneUI();
public:
    ecs::Scene* loadScene(std::string sceneName) override {
        if (sceneName=="scene2") {
            return loadScene2();
        }else if (sceneName=="scene1") {
            return loadScene1();
        }else if (sceneName=="scene_ui") {
            return loadSceneUI();
        }
        return nullptr;
    }
    bool unloadScene(std::string sceneName) override {
        if (sceneName=="scene1") {
            unloadScene1();
        }else if (sceneName=="scene2") {
            unloadScene2();
        }
        return true;
    }
    void init() override;
};


#endif //GAMEENGINE_DEMOGAMEAPPLICATION_H
