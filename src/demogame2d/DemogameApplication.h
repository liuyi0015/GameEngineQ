//
// Created by XL0002 on 2026/7/17.
//

#ifndef GAMEENGINE_DEMOGAMEAPPLICATION_H
#define GAMEENGINE_DEMOGAMEAPPLICATION_H
#include "systems/CameraInputListenerSystem.h"
#include "GameComponents.h"
#include "systems/MoveSystem.h"
#include "Prefabs.h"
#include "systems/PrintSystem.h"
#include "systems/RotationSystem.h"
#include "../EcsApplication.h"
#include "../ResourceManager.hpp"
#include "../anim/AnimationSystem.h"
#include "../2d/renderer2d-soft/RenderComponents.h"
#include "../2d/physics2d/Physics2DSystem.h"
#include "../2d/renderer2d-soft/Render2dSystem.h"
#include "../2d/transform2d/TransformUtil.h"
#include "../ui/MouseUISystem.h"
#include "systems/JumpSystem.h"

class DemogameApplication:public EcsApplication{
private:

    Scene* loadScene1() {
        auto scene=new Scene();
        for (int i=0;i<100;i++) {
            Entity move_entity=ecs::createEntity(scene);
            ecs::setComponent<ecs::Enabled>(scene, move_entity, ecs::Enabled{true});
            ecs::setComponent<ecs::Name>(scene, move_entity, ecs::Name{"entity0"});
            ecs::setComponent<TransformComp>(scene, move_entity, TransformComp{Position{50.0f+i*10, 50.0f}, Rotation{}, Scale{}});
            ecs::setComponent<DrawableFlag>(scene, move_entity, DrawableFlag{1,{255,255,255,255}});
            ecs::setComponent<RectRendererFlag>(scene, move_entity,{10,10});
            ecs::setComponent<MoveFlag>(scene, move_entity,{400.0f, 400.0f,50,50, 200.0f});
        }
        Entity circle1=Prefabs::physicsCircle(scene);
        Entity circle2=Prefabs::physicsCircle(scene);
        ecs::setComponent<TransformComp>(scene,circle2,{160,300});
        ecs::setComponent<JumpFlag>(scene,circle2,{});
        Prefabs::staticGround(scene);
        Entity camera1=Prefabs::camera(scene);
        ecs::setComponent<CameraInputListenerFlag>(scene, camera1, {});
        auto cameraInputListenerSystem=new CameraInputListenerSystem(scene);
        ecs::addSystem(scene,cameraInputListenerSystem,{});
        auto physics2dSystem=new Physics2DSystem(scene);
        SystemId physics_system_id=ecs::addSystem(scene,physics2dSystem,{1});
        auto jumpSystem=new JumpSystem(scene,physics_system_id);
        ecs::addSystem(scene,jumpSystem,{2});
        // auto scalerSystem=new ScalerSystem(scene);
        // ecs::addSystem(scene,scalerSystem,{});
        auto moveSystem=new MoveSystem(scene);
        ecs::addSystem(scene,moveSystem,{});
        auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        auto render2dSystem=new Render2dSystem(scene, renderer,camera1);
        ecs::addSystem(scene,render2dSystem,{});
        return scene;
    }
    Scene* loadScene2() {
        auto scene=new Scene();
        Entity img1=Prefabs::staticImage(scene);
        Entity anim1=Prefabs::anim1(scene,img1);
        ecs::setComponent<RotationFlag>(scene, img1, RotationFlag{100});
        Prefabs::button(scene,img1);
        //摄像机的视口大小与transform(scale)无关，只看CameraComp
        Entity camera1=Prefabs::camera(scene);
        ecs::setComponent<CameraInputListenerFlag>(scene, camera1, {});
        auto cameraInputListenerSystem=new CameraInputListenerSystem(scene);
        ecs::addSystem(scene,cameraInputListenerSystem,{});
        //systems
        auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        auto render2dSystem=new Render2dSystem(scene, renderer,camera1);
        ecs::addSystem(scene,render2dSystem,{});
        auto animationSystem=new AnimationSystem(scene,renderer);
        ecs::addSystem(scene,animationSystem,{});
        auto rotationSystem=new RotationSystem(scene);
        ecs::addSystem(scene,rotationSystem,{});
        auto print_system1 = new PrintSystem(scene,1);
        ecs::addSystem(scene,print_system1,{10,0,0,0});
        auto print_system2 = new PrintSystem(scene,2);
        ecs::addSystem(scene,print_system2,{});

        return scene;
    }
public:
    Scene* loadScene(std::string sceneName) override {
        if (sceneName=="scene2") {
            return loadScene2();
        }else if (sceneName=="scene1") {
            return loadScene1();
        }
        return nullptr;
    }
    void init() override {
        TTF_Font* font1=TTF_OpenFont("assets/1.ttf", 24);
        ResourceManager::getInstance().getFontCache().set("font1", font1);
        auto gScene=new Scene();
        auto mouseColliderSystem=new MouseUISystem(gScene);
        ecs::addSystem(gScene,mouseColliderSystem,{});
        global_scenes.push_back(gScene);
        EcsApplication::init();
        switch_scene("scene2");
    }
};


#endif //GAMEENGINE_DEMOGAMEAPPLICATION_H
