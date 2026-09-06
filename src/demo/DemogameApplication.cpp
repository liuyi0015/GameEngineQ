//
// Created by XL0002 on 2026/7/17.
//

#include "DemogameApplication.h"
#include "GameComponents.h"
#include "../core/ecs/BaseComponents.h"
#include "../core/transform/transform2d/Transform2dComponents.h"
#include "../../logs/Prefabs.h"
#include "../Config.h"
#include "simple-systems/ChangeTransformSystem.h"
#include "../anim/AnimationSystem.h"
#include "render-blueprint/Render2DComponents.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "simple-systems/PrintSystem.h"
#include "../core/ResourceManager.hpp"

// ecs::Scene *DemogameApplication::loadScene1() {
//
//     auto* scene=new ecs::Scene("scene1");
//     {
//         for (int i=0;i<10;i++) {
//             Entity move_entity=ecs::createEntity(scene);
//             ecs::setComponent<ecs::Enabled>(scene, move_entity, ecs::Enabled{true});
//             ecs::setComponent<ecs::Name>(scene, move_entity, ecs::Name{"entity0"});
//             ecs::setComponent<TransformComp>(scene, move_entity, TransformComp{Position{50.0f+i*10, 50.0f}, Rotation{}, Scale{}});
//             Mesh::Shape2D rect=Mesh::createRect({0,0},{10,10});
//             ecs::setComponent<Drawable2DFlag>(scene, move_entity, Drawable2DFlag{1,""});
//             ecs::setComponent<MoveFlag>(scene, move_entity,{400.0f, 400.0f,50,50, 200.0f});
//         }
//         auto* changeTransformSystem=new ChangeTransformSystem(scene);
//         ecs::addSystem(scene,changeTransformSystem,{});
//     }
//     {
//         Entity circle1=Prefabs::physicsCircle(scene);
//         Entity circle2=Prefabs::physicsCircle(scene);
//         ecs::setComponent<TransformComp>(scene,circle2,{160,300});
//         ecs::setComponent<JumpFlag>(scene,circle2,{});
//         Prefabs::staticGround(scene);
//
//         // auto* physics2dSystem=new Physics2DSystem(scene);
//         // SystemId physics_system_id=ecs::addSystem(scene,physics2dSystem,{1});
//         // auto* jumpSystem=new JumpSystem(scene,physics_system_id);
//         // ecs::addSystem(scene,jumpSystem,{2});
//     }
//     {
//         Entity camera1=Prefabs::camera(scene);
//         ecs::setComponent<CameraInputListenerFlag>(scene, camera1, {});
//         auto* cameraInputListenerSystem=new CameraInputListenerSystem(scene);
//         ecs::addSystem(scene,cameraInputListenerSystem,{});
//     }
//     {
//         auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
//         auto config=ApplicationContext::getInstance().get<Config>("config");
//         SDL_Texture* target=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,
//             SDL_TEXTUREACCESS_TARGET,config.LOGIC_WIDTH,config.LOGIC_HEIGHT);
//         //要显示到屏幕的才直接给compositor
//         compositor.addTarget(scene,target);
//         auto* render2dSystem=new Render2dSystem(scene,target,camera1);
//         ecs::addSystem(scene,render2dSystem,{});
//     }
//
//     return scene;
// }
// void DemogameApplication::unloadScene1() {
//     std::cout<<"unloadScene1"<<std::endl;
//     compositor.removeTarget(getSceneByName("scene1"));
// }
class Scene1:public ecs::Scene{
public:
    Scene1() {
        //
        // {
        //
        //     Entity img1=Prefabs::staticImage(scene);
        //     Entity anim1=Prefabs::anim1(scene,img1);
        //     ecs::setComponent<RotationFlag>(scene, img1, RotationFlag{20});
        //     auto* rotationSystem=new ChangeTransformSystem(scene);
        //     ecs::addSystem(scene,rotationSystem,{});
        // }
        // {
        //
        //     //摄像机的视口大小与transform(scale)无关，只看CameraComp
        //     Entity camera2=Prefabs::camera(scene);
        //     ecs::setComponent<CameraInputListenerFlag>(scene, camera2, {});
        //     auto* cameraInputListenerSystem=new CameraInputListenerSystem(scene);
        //     ecs::addSystem(scene,cameraInputListenerSystem,{});
        // }
        {
            //顺序test
            auto* print_system1 = new PrintSystem(this,1);
            ecs::addSystem(this,print_system1,{10,0,0,0});
            auto* print_system2 = new PrintSystem(this,2);
            ecs::addSystem(this,print_system2,{});
        }
        // {
        //     //render
        //     auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        //     auto* animationSystem=new AnimationSystem(scene,renderer);
        //     ecs::addSystem(scene,animationSystem,{});
        //     auto config=ApplicationContext::getInstance().get<Config>("config");
        //     SDL_Texture* target=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,
        //         SDL_TEXTUREACCESS_TARGET,config.LOGIC_WIDTH,config.LOGIC_HEIGHT);
        //     //要显示到屏幕的才直接给compositor
        //     compositor.addTarget(scene,target);
        //     //todo 小屏幕渲染目标(不给compositor)
        //     auto* render2dSystem=new Render2dSystem(scene,target,camera2);
        //     ecs::addSystem(scene,render2dSystem,{});
        //
        // }
    }
    ~Scene1() {
        std::cout<<"unloadScene2"<<std::endl;
        // compositor.removeTarget(getSceneByName("scene2"));

    }
};
void DemogameApplication::init() {
    // TTF_Font* font1=TTF_OpenFont("assets/1.ttf", 24);
    // ResourceManager::getInstance().getFontCache().set("font1", font1);
    scene=new Scene1();
}
