//
// Created by XL0002 on 2026/8/5.
//

#include "DemoGameApplication.h"

#include "HelloWorldSystem.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "../ResourceManager.hpp"
#include "../demogame2d/systems/PrintSystem.h"
#include "../ecs/Util.h"
static Scene* loadScene1() {
    auto scene=new Scene();
    auto hello_world_system = new HelloWorldSystem(scene);
    ecs::addSystem(scene,hello_world_system,{});
    return scene;
}
Scene* DemoGameApplication::loadScene(std::string sceneName) {
     if (sceneName=="scene1") {
        return loadScene1();
    }
     return nullptr;
}

void DemoGameApplication::init() {
    // global_systems.push_back({std::make_shared<MouseColliderSystem>(scene),1});
    EcsApplication::init();
    switch_scene("scene1");
}
