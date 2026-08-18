//
// Created by XL0002 on 2026/7/17.
//

#include "DemogameApplication.h"
#include "GameComponents.h"
#include "../ecs/BaseComponents.h"
#include "../2d/transform2d/Transform2dComponents.h"
#include "../2d/renderer2d/RenderComponents.h"
#include "Prefabs.h"
#include "systems/CameraInputListenerSystem.h"
#include "../2d/physics2d/Physics2DSystem.h"
#include "systems/JumpSystem.h"
#include "../Config.h"
#include "simple-systems/RotationSystem.h"
#include "../2d/renderer2d/Render2dSystem.h"
#include "../anim/AnimationSystem.h"
#include "../ui/MouseUISystem.h"
#include "../ui/UIRenderSystem.h"
#include "systems/ButtonCallbackSystem.h"

ecs::Scene *DemogameApplication::loadScene1() {

    auto* scene=new ecs::Scene("scene1");
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
    auto* cameraInputListenerSystem=new CameraInputListenerSystem(scene);
    ecs::addSystem(scene,cameraInputListenerSystem,{});
    auto* physics2dSystem=new Physics2DSystem(scene);
    SystemId physics_system_id=ecs::addSystem(scene,physics2dSystem,{1});
    auto* jumpSystem=new JumpSystem(scene,physics_system_id);
    ecs::addSystem(scene,jumpSystem,{2});
    // auto scalerSystem=new ScalerSystem(scene);
    // ecs::addSystem(scene,scalerSystem,{});
    auto* moveSystem=new MoveSystem(scene);
    ecs::addSystem(scene,moveSystem,{});
    auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
    auto config=ApplicationContext::getInstance().get<Config>("config");
    SDL_Texture* target=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,config.LOGIC_WIDTH,config.LOGIC_HEIGHT);
    //要显示到屏幕的才直接给compositor
    compositor.addTarget(scene,target);
    auto* render2dSystem=new Render2dSystem(scene,target,camera1);
    ecs::addSystem(scene,render2dSystem,{});

    return scene;
}
void DemogameApplication::unloadScene1() {
    std::cout<<"unloadScene1"<<std::endl;
    compositor.removeTarget(getSceneByName("scene1"));
}
ecs::Scene *DemogameApplication::loadScene2() {
    auto* scene=new ecs::Scene("scene2");
    Entity img1=Prefabs::staticImage(scene);
    Entity anim1=Prefabs::anim1(scene,img1);
    ecs::setComponent<RotationFlag>(scene, img1, RotationFlag{20});
    //摄像机的视口大小与transform(scale)无关，只看CameraComp
    Entity camera1=Prefabs::camera(scene);
    ecs::setComponent<CameraInputListenerFlag>(scene, camera1, {});
    auto* cameraInputListenerSystem=new CameraInputListenerSystem(scene);
    ecs::addSystem(scene,cameraInputListenerSystem,{});
    //systems
    auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
    auto* animationSystem=new AnimationSystem(scene,renderer);
    ecs::addSystem(scene,animationSystem,{});
    auto* rotationSystem=new RotationSystem(scene);
    ecs::addSystem(scene,rotationSystem,{});
    auto* print_system1 = new PrintSystem(scene,1);
    ecs::addSystem(scene,print_system1,{10,0,0,0});
    auto* print_system2 = new PrintSystem(scene,2);
    ecs::addSystem(scene,print_system2,{});
    auto config=ApplicationContext::getInstance().get<Config>("config");
    SDL_Texture* target=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,config.LOGIC_WIDTH,config.LOGIC_HEIGHT);
    //要显示到屏幕的才直接给compositor
    compositor.addTarget(scene,target);
    //todo 小屏幕渲染目标(不给compositor)
    auto* render2dSystem=new Render2dSystem(scene,target,camera1);
    ecs::addSystem(scene,render2dSystem,{});
    return scene;
}

void DemogameApplication::unloadScene2() {
    std::cout<<"unloadScene2"<<std::endl;
    compositor.removeTarget(getSceneByName("scene2"));
}

ecs::Scene* DemogameApplication::loadSceneUI() {
    auto* scene=new ecs::Scene("scene_ui");

    Prefabs::button(scene,std::nullopt);
    auto* buttonRectChangeColorSystem=new ButtonCallbackSystem(scene);
    ecs::addSystem(scene,buttonRectChangeColorSystem,{});
    auto mouseColliderSystem=new MouseUISystem(scene);
    ecs::addSystem(scene,mouseColliderSystem,{});

    auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
    auto config=ApplicationContext::getInstance().get<Config>("config");
    SDL_Texture* target=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,config.LOGIC_WIDTH,config.LOGIC_HEIGHT);
    //要显示到屏幕的才直接给compositor
    compositor.addTarget(scene,target);
    auto uiRenderSystem=new UIRenderSystem(scene,target);
    ecs::addSystem(scene,uiRenderSystem,{});
    return scene;
}

void DemogameApplication::init() {
    TTF_Font* font1=TTF_OpenFont("assets/1.ttf", 24);
    ResourceManager::getInstance().getFontCache().set("font1", font1);
    // auto gScene=new ecs::Scene("global");
    // global_scenes.push_back(gScene);
    EcsApplication::init();
    addScene("scene1");
    addScene("scene2");
    addScene("scene_ui");
}
