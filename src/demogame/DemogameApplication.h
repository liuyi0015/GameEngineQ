//
// Created by XL0002 on 2026/7/17.
//

#ifndef GAMEENGINE_DEMOGAMEAPPLICATION_H
#define GAMEENGINE_DEMOGAMEAPPLICATION_H
#include "CameraInputListenerSystem.h"
#include "GameComponents.h"
#include "MoveSystem.h"
#include "Prefabs.h"
#include "PrintSystem.h"
#include "RotationSystem.h"
#include "ScalerSystem.h"
#include "../EcsApplication.h"
#include "../ResourceManager.hpp"
#include "../anim/AnimationComponents.h"
#include "../anim/AnimationSystem.h"
#include "../2d/renderer2d/RenderComponents.h"
#include "../2d/physics2d/Physics2DSystem.h"
#include "../2d/renderer2d/Render2dSystem.h"
#include "../2d/transform2d/TransformUtil.h"
#include "../ui/MouseColliderSystem.h"

class DemogameApplication:public EcsApplication{
private:

    std::shared_ptr<Scene> loadScene1() {
        auto scene=std::make_shared<Scene>();
        for (int i=0;i<100;i++) {
            Entity move_entity=ecs::createEntity(scene);
            ecs::setComponent<ecs::Enabled>(scene, move_entity, ecs::Enabled{true});
            ecs::setComponent<ecs::Name>(scene, move_entity, ecs::Name{"entity0"});
            ecs::setComponent<Transform>(scene, move_entity, Transform{Position{50.0f+i*10, 50.0f}, Rotation{}, Scale{}});
            ecs::setComponent<DrawableFlag>(scene, move_entity, DrawableFlag{1,{255,255,255,255}});
            ecs::setComponent<RectRendererFlag>(scene, move_entity,{100,100});
            ecs::setComponent<MoveFlag>(scene, move_entity,{400.0f, 400.0f,50,50, 200.0f});
        }
        Prefabs::physicsCircle(scene);
        Entity camera1=Prefabs::camera(scene);
        ecs::setComponent<CameraInputListenerFlag>(scene, camera1, {});
        // auto scalerSystem=std::make_shared<ScalerSystem>(scene);
        // ecs::addSystem(scene,scalerSystem,{});
        auto cameraInputListenerSystem=std::make_shared<CameraInputListenerSystem>(scene);
        ecs::addSystem(scene,cameraInputListenerSystem,{});
        auto physics2dSystem=std::make_shared<Physics2DSystem>(scene);
        ecs::addSystem(scene,physics2dSystem,{});
        auto moveSystem=std::make_shared<MoveSystem>(scene);
        ecs::addSystem(scene,moveSystem,{});
        auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        auto render2dSystem=std::make_shared<Render2dSystem>(scene, renderer,camera1);
        ecs::addSystem(scene,render2dSystem,{});
        return scene;
    }
    std::shared_ptr<Scene> loadScene2() {
        auto scene=std::make_shared<Scene>();
        Entity img1=Prefabs::staticImage(scene);
        Entity anim1=Prefabs::anim1(scene,img1);
        ecs::setComponent<RotationFlag>(scene, anim1, RotationFlag{100});
        Prefabs::button(scene,img1);
        //摄像机的视口大小与transform(scale)无关，只看CameraComp
        Entity camera1=Prefabs::camera(scene);
        //systems
        auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        auto render2dSystem=std::make_shared<Render2dSystem>(scene, renderer,camera1);
        ecs::addSystem(scene,render2dSystem,{});
        auto animationSystem=std::make_shared<AnimationSystem>(scene,renderer);
        ecs::addSystem(scene,animationSystem,{});
        auto rotationSystem=std::make_shared<RotationSystem>(scene);
        ecs::addSystem(scene,rotationSystem,{});
        auto print_system1 = std::make_shared<PrintSystem>(scene,1);
        ecs::addSystem(scene,print_system1,{10,0,0,0});
        auto print_system2 = std::make_shared<PrintSystem>(scene,2);
        ecs::addSystem(scene,print_system2,{});

        return scene;
    }
public:
    void init() override {
        TTF_Font* font1=TTF_OpenFont("assets/1.ttf", 24);
        ResourceManager::getInstance().getFontCache().set("font1", std::shared_ptr<TTF_Font>(font1, TTF_CloseFont));
        scenes["scene1"]=loadScene1();
        scenes["scene2"]=loadScene2();
        scene=scenes["scene2"];

        global_systems.push_back({std::make_shared<MouseColliderSystem>(scene),1});
        EcsApplication::init();
    }
};


#endif //GAMEENGINE_DEMOGAMEAPPLICATION_H
