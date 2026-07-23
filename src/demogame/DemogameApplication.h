//
// Created by XL0002 on 2026/7/17.
//

#ifndef GAMEENGINE_DEMOGAMEAPPLICATION_H
#define GAMEENGINE_DEMOGAMEAPPLICATION_H
#include "GameComponents.h"
#include "MoveSystem.h"
#include "../EcsApplication.h"
#include "../ResourceManager.hpp"
#include "../physics2d/Physics2DSystem.h"
#include "../renderer/RenderSystem.h"
#include "../renderer/RenderComponents.h"
#include "../ui/ButtonFactory.h"
#include "../ui/MouseColliderSystem.h"

class DemogameApplication:public EcsApplication{
private:

    static std::shared_ptr<Scene> loadScene1() {
        auto scene=std::make_shared<Scene>();
        for (int i=0;i<100;i++) {
            Entity move_entity=scene->createEntity();
            scene->setComponent<ecs::Enabled>(move_entity, ecs::Enabled{true});
            scene->setComponent<Name>(move_entity, Name{"entity0"});
            scene->setComponent<ecs::Transform>(move_entity, ecs::Transform{ecs::Position{50.0f+i*10, 50.0f}, ecs::Rotation{}, ecs::Scale{}});
            scene->setComponent<DrawableFlag>(move_entity, DrawableFlag{1,{255,255,255,255}});
            scene->setComponent<RectRendererFlag>(move_entity,{100,100});
            scene->setComponent<MoveFlag>(move_entity,{400.0f, 400.0f,50,50, 200.0f});
        }
        Entity circle_entity=scene->createEntity();
        scene->setComponent<ecs::Enabled>(circle_entity, ecs::Enabled{true});
        scene->setComponent<Name>(circle_entity, Name{"entity1"});
        scene->setComponent<ecs::Transform>(circle_entity, ecs::Transform{ecs::Position{200.0f, 100.0f}, ecs::Rotation{}, ecs::Scale{}});
        scene->setComponent<DrawableFlag>(circle_entity,DrawableFlag{0,{255,0,0,255}});
        scene->setComponent<CircleRendererFlag>(circle_entity,{  50.0f,32});
        scene->setComponent<RigidBodyComp>(circle_entity,{b2_dynamicBody});

        scene->systems.push_back(std::make_shared<MoveSystem>(scene));
        // scene->systems.push_back(std::make_shared<Physics2DSystem>(scene));
        return scene;
    }
    static std::shared_ptr<Scene> loadScene2() {
        auto scene=std::make_shared<Scene>();

        Entity img1_entity=scene->createEntity();
        scene->setComponent<Name>(img1_entity, Name{"img1"});
        scene->setComponent<ecs::Enabled>(img1_entity, ecs::Enabled{true});
        scene->setComponent<ecs::Transform>(img1_entity, ecs::Transform{ecs::Position{100.0f, 0.0f}, ecs::Rotation{0}, ecs::Scale{}});
        scene->setComponent<DrawableFlag>(img1_entity,DrawableFlag{0,{255,255,255,255}});
        scene->setComponent<ImageRendererFlag>(img1_entity,{ "img1-tex",988,852});

        Entity textBtn1_entity=scene->createEntity();
        scene->setComponent<Name>(textBtn1_entity, Name{"text1"});
        scene->setComponent<ecs::Enabled>(textBtn1_entity, ecs::Enabled{true});
        scene->setComponent<ecs::Transform>(textBtn1_entity, ecs::Transform{
            ecs::Position{400.0f, 100.0f}, ecs::Rotation{}, ecs::Scale{},
            std::make_optional(img1_entity)
        });
        scene->setComponent<DrawableFlag>(textBtn1_entity,DrawableFlag{1,{255,255,255,255}});
        scene->setComponent<TextRendererFlag>(textBtn1_entity,{ "我是按钮，点我",
            "font1","text1-surf","text1-tex"});
        // scene->setComponent<RectRendererFlag>(textBtn1_entity,RectRendererFlag{100,100});
        scene->setComponent<MouseColliderFlag>(textBtn1_entity,{100,100,0,true});
        scene->setComponent<LeftMouseColliderEvents>(textBtn1_entity,{"say hello"});
        EventDispatcher::getInstance().subscribe("say hello",[](const std::any& param) {
            std::cout<<"hello,GameEngineQ!"<<std::endl;
        });
        //systems

        return scene;
    }
public:
    void init() override {
        TTF_Font* font1=TTF_OpenFont("assets/1.ttf", 24);
        ResourceManager::getInstance().getFontCache().set("font1", std::shared_ptr<TTF_Font>(font1, TTF_CloseFont));
        scenes["scene1"]=loadScene1();
        scenes["scene2"]=loadScene2();
        scene=scenes["scene2"];
        auto *renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        global_systems.push_back(std::make_shared<RenderSystem>(scene, renderer));
        global_systems.push_back(std::make_shared<MouseColliderSystem>(scene));
    }
};


#endif //GAMEENGINE_DEMOGAMEAPPLICATION_H
