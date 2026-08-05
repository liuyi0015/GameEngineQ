//
// Created by XL0002 on 2026/8/5.
//

#include "DemoGameApplication.h"

#include "HelloWorldSystem.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "../ResourceManager.hpp"
#include "../demogame2d/PrintSystem.h"
#include "../ecs/Util.h"
static std::shared_ptr<Scene> loadScene1() {
    auto scene=std::make_shared<Scene>();
    auto print_system1 = std::make_shared<PrintSystem>(scene,1);
    ecs::addSystem(scene,print_system1,{10,0,0,0});
    auto print_system2 = std::make_shared<PrintSystem>(scene,2);
    ecs::addSystem(scene,print_system2,{});
    auto hello_world_system = std::make_shared<HelloWorldSystem>(scene);
    ecs::addSystem(scene,hello_world_system,{});
    return scene;
}
void DemoGameApplication::init() {
    TTF_Font* font1=TTF_OpenFont("assets/1.ttf", 24);
    ResourceManager::getInstance().getFontCache().set("font1", std::shared_ptr<TTF_Font>(font1, TTF_CloseFont));
    scenes["scene1"]=loadScene1();
    // scenes["scene2"]=loadScene2();
    scene=scenes["scene1"];
    // global_systems.push_back({std::make_shared<MouseColliderSystem>(scene),1});
    EcsApplication::init();
}
