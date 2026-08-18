//
// Created by XL0002 on 2026/7/28.
//

#include "Prefabs.h"

#include "GameComponents.h"
#include "../ecs/BaseComponents.h"

#include "../2d/renderer2d/RenderComponents.h"
#include "../2d/transform2d/Transform2dComponents.h"
#include "../2d/physics2d/PhysicsComponents.h"
#include "../ui/UIComponents.h"
#include "../anim/AnimationComponents.h"
#include "../EventDispatcher.h"
#include "../ecs/Util.h"
#include"../3d/transform3d/Transform3dComponents.h"
#include "../3d/render3d/RenderComponents.h"

Entity Prefabs::physicsCircle(ecs::Scene* scene) {

    Entity circle_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,circle_entity, ecs::Enabled{true});
    ecs::setComponent<ecs::Name>(scene,circle_entity, ecs::Name{"entity1"});
    ecs::setComponent<TransformComp>(scene,circle_entity, TransformComp{Position{200.0f, 100.0f}});
    ecs::setComponent<DrawableFlag>(scene,circle_entity,DrawableFlag{0,{255,0,0,255}});
    ecs::setComponent<CircleRendererFlag>(scene,circle_entity,{  50.0f,32});
    ecs::setComponent<RigidBodyComp>(scene,circle_entity,{b2_dynamicBody});
    return circle_entity;
}
Entity Prefabs::staticGround(ecs::Scene* scene) {
    Entity ground_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,ground_entity, ecs::Enabled{true});
    ecs::setComponent<ecs::Name>(scene,ground_entity, ecs::Name{"ground"});
    ecs::setComponent<TransformComp>(scene,ground_entity, TransformComp{Position{100.0f, 500.0f}});
    ecs::setComponent<DrawableFlag>(scene,ground_entity,DrawableFlag{0,{255,0,0,255}});
    ecs::setComponent<RectRendererFlag>(scene,ground_entity,{  500.0f,32});
    ecs::setComponent<RigidBodyComp>(scene,ground_entity,{b2_staticBody});
    return ground_entity;
}
Entity Prefabs::staticImage(ecs::Scene* scene) {
    //image
    Entity img1_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Name>(scene,img1_entity, ecs::Name{"img1"});
    ecs::setComponent<ecs::Enabled>(scene,img1_entity, ecs::Enabled{true});
    ecs::setComponent<TransformComp>(scene,img1_entity, TransformComp{Position{100.0f, 0.0f}, Rotation{30}, Scale{}});
    ecs::setComponent<DrawableFlag>(scene,img1_entity,DrawableFlag{0,{255,255,255,255}});
    ecs::setComponent<ImageRendererFlag>(scene,img1_entity,{ "assets/1.png","img1-tex",988,852});
    return img1_entity;
}

Entity Prefabs::anim1(ecs::Scene* scene,std::optional<Entity> parent) {
    //anim
    Entity anim1_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Name>(scene,anim1_entity, ecs::Name{"anim1"});
    ecs::setComponent<ecs::Enabled>(scene,anim1_entity, ecs::Enabled{true});
    ecs::setComponent<TransformComp>(scene,anim1_entity, TransformComp{{{500.0f, 200.0f}, {0}, {1,1.5f}},parent});
    ecs::setComponent<DrawableFlag>(scene,anim1_entity,DrawableFlag{0,{255,255,255,255}});
    ecs::setComponent<ImageRendererFlag>(scene,anim1_entity,{ "","anim1-tex",600,550});
    ecs::setComponent<FrameAnimatorFlag>(scene,anim1_entity,{"anim1","assets/1.gif",true});
    return anim1_entity;
}

Entity Prefabs::camera(ecs::Scene* scene) {
    Entity camera_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,camera_entity, ecs::Enabled{true});
    ecs::setComponent<TransformComp>(scene,camera_entity, TransformComp{{{0,0},{0},{1,1}},std::nullopt});
    ecs::setComponent<CameraComp>(scene,camera_entity,{800,600});
    return camera_entity;
}

Entity Prefabs::button(ecs::Scene* scene, const std::optional<Entity> parent) {
    //textBtn{rect,text,collider}
    Entity textBtn1_entity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Name>(scene,textBtn1_entity, ecs::Name{"textBtn1"});
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_entity, ecs::Enabled{true});
    ecs::setComponent<TransformComp>(scene,textBtn1_entity, TransformComp{
        Position{400.0f, 100.0f}, Rotation{45}, Scale{},parent
    });
    Entity textBtn1_rect_subEntity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_rect_subEntity, ecs::Enabled{true});
    ecs::setComponent<ecs::Name>(scene,textBtn1_rect_subEntity, ecs::Name{"textBtn1_rect"});
    ecs::setComponent<TransformComp>(scene,textBtn1_rect_subEntity, TransformComp{
        Position{0.0f, 0.0f}, Rotation{}, Scale{},textBtn1_entity
    });
    ecs::setComponent<DrawableFlag>(scene,textBtn1_rect_subEntity,DrawableFlag{1,{255,0,255,255}});
    ecs::setComponent<RectRendererFlag>(scene,textBtn1_rect_subEntity,{ 100,100});
    ecs::setComponent<HoverColorFlag>(scene,textBtn1_rect_subEntity,HoverColorFlag{{255,0,0,255},{0,0,255,255}});

    Entity textBtn1_text_subEntity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_text_subEntity, ecs::Enabled{true});
    ecs::setComponent<TransformComp>(scene,textBtn1_text_subEntity, TransformComp{
        Position{0.0f, 0.0f}, Rotation{}, Scale{},textBtn1_entity
    });
    ecs::setComponent<DrawableFlag>(scene,textBtn1_text_subEntity,DrawableFlag{2,{255,255,255,255}});
    ecs::setComponent<TextRendererFlag>(scene,textBtn1_text_subEntity,{ "我是按钮，点我退出",
        "font1","text1-tex"});

    Entity textBtn1_collider_subEntity=ecs::createEntity(scene);
    ecs::setComponent<ecs::Enabled>(scene,textBtn1_collider_subEntity, ecs::Enabled{true});
    ecs::setComponent<TransformComp>(scene,textBtn1_collider_subEntity, TransformComp{
        Position{0.0f, 0.0f}, Rotation{}, Scale{},textBtn1_entity
    });
    Triangle triangles_raw[]={{{0,0},{0,100},{100,100}},
        {{0,0},{100,0},{100,100}}};
    auto triangles=std::vector<Triangle>(triangles_raw,triangles_raw+sizeof(triangles_raw)/sizeof(Triangle));
    ecs::setComponent<MouseColliderFlag>(scene,textBtn1_collider_subEntity,{triangles,0,true});
    ecs::setComponent<LeftMouseColliderEvents>(scene,textBtn1_collider_subEntity,{"say hello"});
    ecs::setComponent<MoveMouseColliderEvents>(scene,textBtn1_collider_subEntity,{"","",
        "text1-tex hover","text1-tex unhover"});

    return textBtn1_entity;
}

Entity Prefabs::cube(ecs::Scene* scene) {
    Entity entity=ecs::createEntity(scene);
    Position3D ps[8]={
        {0,0,0},
        {0,0,1},
        {0,1,0},
        {0,1,1},
        {1,0,0},
        {1,0,1},
        {1,1,0},
        {1,1,1}
    };
    std::vector<Vertex3D> vertices;
    for (auto & p : ps) {
        vertices.push_back(Vertex3D{p,SDL_Color{255,255,255},{0,0}});
    }
    ecs::setComponent<Drawable3DFlag>(scene,entity, {SDL_Color{255,255,255,255},std::move(vertices)});
    ecs::setComponent<Transform3D>(scene,entity, {});
    return entity;
}

