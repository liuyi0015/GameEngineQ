//
// Created by XL0002 on 2026/7/28.
//

#include "Prefabs.h"
#include "../ecs/BaseComponents.h"

#include "../2d/renderer2d/RenderComponents.h"
#include "../2d/transform2d/Transform2dComponents.h"
#include "../2d/physics2d/PhysicsComponents.h"
#include "../ui/UIComponents.h"
#include "../anim/AnimationComponents.h"
#include "../EventDispatcher.h"
#include "../ecs/Util.h"

Entity Prefabs::physicsCircle(const std::shared_ptr<Scene> &scene) {

    Entity circle_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,circle_entity, ecs::Enabled{true});
    ecs::setComponent<ecs::Name>(scene,circle_entity, ecs::Name{"entity1"});
    ecs::setComponent<Transform>(scene,circle_entity, Transform{Position{200.0f, 100.0f}});
    ecs::setComponent<DrawableFlag>(scene,circle_entity,DrawableFlag{0,{255,0,0,255}});
    ecs::setComponent<CircleRendererFlag>(scene,circle_entity,{  50.0f,32});
    ecs::setComponent<RigidBodyComp>(scene,circle_entity,{b2_dynamicBody});
    return circle_entity;
}
Entity Prefabs::staticImage(const std::shared_ptr<Scene> &scene) {
    //image
    Entity img1_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Name>(scene,img1_entity, ecs::Name{"img1"});
    ecs::setComponent<ecs::Enabled>(scene,img1_entity, ecs::Enabled{true});
    ecs::setComponent<Transform>(scene,img1_entity, Transform{Position{100.0f, 0.0f}, Rotation{0}, Scale{}});
    ecs::setComponent<DrawableFlag>(scene,img1_entity,DrawableFlag{0,{255,255,255,255}});
    ecs::setComponent<ImageRendererFlag>(scene,img1_entity,{ "img1-tex",988,852});
    return img1_entity;
}

Entity Prefabs::anim1(const std::shared_ptr<Scene> &scene) {
    //anim
    Entity anim1_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Name>(scene,anim1_entity, ecs::Name{"anim1"});
    ecs::setComponent<ecs::Enabled>(scene,anim1_entity, ecs::Enabled{true});
    ecs::setComponent<Transform>(scene,anim1_entity, Transform{Position{500.0f, 400.0f}, Rotation{0}, Scale{}});
    ecs::setComponent<DrawableFlag>(scene,anim1_entity,DrawableFlag{0,{255,255,255,255}});
    ecs::setComponent<ImageRendererFlag>(scene,anim1_entity,{ "anim1-tex",600,550});
    ecs::setComponent<FrameAnimatorFlag>(scene,anim1_entity,{"anim1","assets/1.gif",true});
    return anim1_entity;
}

Entity Prefabs::camera(const std::shared_ptr<Scene> &scene) {
    Entity camera_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,camera_entity, ecs::Enabled{true});
    ecs::setComponent<Transform>(scene,camera_entity,Transform{{0,0}});
    ecs::setComponent<CameraComp>(scene,camera_entity,{800,600});
    return camera_entity;
}

Entity Prefabs::button(const std::shared_ptr<Scene> &scene, const std::optional<Entity> parent) {
    //textBtn{rect,text,collider}
    Entity textBtn1_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Name>(scene,textBtn1_entity, ecs::Name{"textBtn1"});
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_entity, ecs::Enabled{true});
    ecs::setComponent<Transform>(scene,textBtn1_entity, Transform{
        Position{400.0f, 100.0f}, Rotation{}, Scale{},parent
    });
    Entity textBtn1_rect_subEntity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_rect_subEntity, ecs::Enabled{true});
    ecs::setComponent<ecs::Name>(scene,textBtn1_rect_subEntity, ecs::Name{"textBtn1_rect"});
    ecs::setComponent<Transform>(scene,textBtn1_rect_subEntity, Transform{
        Position{0.0f, 0.0f}, Rotation{}, Scale{},textBtn1_entity
    });
    ecs::setComponent<DrawableFlag>(scene,textBtn1_rect_subEntity,DrawableFlag{1,{255,0,255,255}});
    ecs::setComponent<RectRendererFlag>(scene,textBtn1_rect_subEntity,{ 100,100});

    Entity textBtn1_text_subEntity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_text_subEntity, ecs::Enabled{true});
    ecs::setComponent<Transform>(scene,textBtn1_text_subEntity, Transform{
        Position{0.0f, 0.0f}, Rotation{}, Scale{},textBtn1_entity
    });
    ecs::setComponent<DrawableFlag>(scene,textBtn1_text_subEntity,DrawableFlag{2,{255,255,255,255}});
    ecs::setComponent<TextRendererFlag>(scene,textBtn1_text_subEntity,{ "我是按钮，点我",
        "font1","text1-surf","text1-tex"});

    Entity textBtn1_collider_subEntity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_collider_subEntity, ecs::Enabled{true});
    ecs::setComponent<Transform>(scene,textBtn1_collider_subEntity, Transform{
        Position{0.0f, 0.0f}, Rotation{}, Scale{},textBtn1_entity
    });
    ecs::setComponent<MouseColliderFlag>(scene,textBtn1_collider_subEntity,{100,100,0,true});
    ecs::setComponent<LeftMouseColliderEvents>(scene,textBtn1_collider_subEntity,{"say hello"});
    ecs::setComponent<MoveMouseColliderEvents>(scene,textBtn1_collider_subEntity,{"","",
        "text1-tex change red","text1-tex change blue"});
    EventDispatcher::getInstance().subscribe("say hello",[](const std::any& param) {
            std::cout<<"hello,GameEngineQ!"<<std::endl;
        EventDispatcher::getInstance().publish("quit",nullptr);
        });
    EventDispatcher::getInstance().subscribe("text1-tex change red",[scene,textBtn1_rect_subEntity](const std::any& param) {
        auto drawableFlag=ecs::getComponent<DrawableFlag>(scene,textBtn1_rect_subEntity).value();
        drawableFlag.color={255,0,0,255};
        ecs::setComponent<DrawableFlag>(scene,textBtn1_rect_subEntity,drawableFlag);
    });
    EventDispatcher::getInstance().subscribe("text1-tex change blue",[scene,textBtn1_rect_subEntity](const std::any& param) {
        auto drawableFlag=ecs::getComponent<DrawableFlag>(scene,textBtn1_rect_subEntity).value();
        drawableFlag.color={0,0,255,255};
        ecs::setComponent<DrawableFlag>(scene,textBtn1_rect_subEntity,drawableFlag);
    });
    return textBtn1_entity;
}
