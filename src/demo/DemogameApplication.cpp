//
// Created by XL0002 on 2026/7/17.
//

#include "DemogameApplication.h"
#include "simple-systems/GameComponents.h"
#include "../ecs/BaseComponents.h"
#include "simple-systems/ChangeTransformSystem.h"
#include "../ecs/systems/anim/AnimationSystem.h"
#include "../ecs/systems/render-blueprint/RenderComponents.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "simple-systems/PrintSystem.h"
#include "../core/ResourceManager.hpp"
#include "../ecs/systems/render-blueprint/RenderSystem.h"
#include "../graphics/Shape2DBuilder.h"
#include "../ecs/util/TransformSceneUtil.h"
#include "../graphics/Shape3DBuilder.h"
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
class Scene1:public ecs::Scene{
public:
    Scene1() {
        {
            //普通system
            auto* changeTransformSystem=new ChangeTransformSystem(this);
            ecs::addSystem(this,changeTransformSystem,{});
            //顺序test
            auto* print_system1 = new PrintSystem(this,1);
            ecs::addSystem(this,print_system1,{10,0,0,0});
            auto* print_system2 = new PrintSystem(this,2);
            ecs::addSystem(this,print_system2,{});
        }
        {
            //加载资源
            //文字显示
            TTF_Font* font1=TTF_OpenFont("assets/1.ttf", 24);
            SDL_Surface* textSurface=TTF_RenderText_Blended_Wrapped(font1,"hello,ttf!hello,ttf!hello,ttf!hello,ttf!hello,ttf!hello,ttf!hello,ttf!hello,ttf!hello,ttf!",90,{255,255,255},80);

            auto* img1=IMG_Load("assets/1.png");
            ResourceManager::getInstance().getSurfaceCache().set("img1-tex",img1);
            //动画
            auto anim1=IMG_LoadAnimation("assets/1.gif");
            ResourceManager::getInstance().getAnimationCache().set("anim1",anim1);

        }
        {
            //image
            Entity img1_entity=ecs::createEntity(this);
            ecs::setComponent<ecs::Name>(this,img1_entity, ecs::Name{"img1"});
            ecs::setComponent<ecs::Enabled>(this,img1_entity, ecs::Enabled{true});
            ecs::setComponent<Transform2DComp>(this,img1_entity, Transform2DComp{{{400,0}}});
            Shape2D rect=Shape2DBuilder::createRect({0,0},{988,852});
            Mesh2D mesh;
            for (auto & point : rect.points) {
                VertexAttrib2D v{};
                v.pos={point.x,point.y};
                v.color={1,1,1,1};
                mesh.vertices.push_back(v);
            }
            //注意是先横坐标x后纵坐标y
            mesh.vertices[0].uv={0,0};//左上
            mesh.vertices[1].uv={1,0};//右上
            mesh.vertices[2].uv={1,1};//右下
            mesh.vertices[3].uv={0,1};//左下
            mesh.indices=rect.indices;
            //todo 目前pipeline写死在render2dProcess，这里配置无效
            Material material={"img1-pipeline",{255,255,255,255},"img1-tex"};
            ecs::setComponent<Drawable2DFlag>(this,img1_entity,Drawable2DFlag{0,material,mesh});

            ecs::setComponent<RotationFlag>(this,img1_entity,{-40.0f});
            ecs::setComponent<FrameAnimationFlag>(this,img1_entity,FrameAnimationFlag{"anim1",0,true});
            auto* animationSystem=new AnimationSystem(this);
            ecs::addSystem(this,animationSystem,{});
        }
        {
            //render
            //摄像机的视口大小与transform(scale)无关，只看CameraComp？
            Entity camera1=ecs::createEntity(this);
            ecs::setComponent<ecs::Enabled>(this,camera1, ecs::Enabled{true});
            ecs::setComponent<Transform2DComp>(this,camera1,Transform2DComp{});
            // auto* gpu = ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
            auto* target=new ColorBuffer(800,600);
            ecs::setComponent<Camera2DComp>(this,camera1,{1200,1080,target});

            // ecs::setComponent<CameraInputListenerFlag>(this, camera1, {});
            // auto* cameraInputListenerSystem=new CameraInputListenerSystem(this);
            // ecs::addSystem(this,cameraInputListenerSystem,{});
            auto* renderSystem=new RenderSystem(this);
            ecs::addSystem(this, renderSystem, {});

        }
    }
    ~Scene1() {
        std::cout<<"析构Scene1"<<std::endl;
    }
};
class Scene2:public ecs::Scene {
public:
    Scene2() {
        {
            Entity cube=ecs::createEntity(this);
            ecs::setComponent<ecs::Enabled>(this,cube, ecs::Enabled{true});
            ecs::setComponent<Transform3DComp>(this,cube,Transform3DComp{{{0,0,-5},
                {30,45,30},{100,100,100}}});
            Shape3D shape=Shape3DBuilder::createCube();
            Mesh3D mesh;
            for (auto& point : shape.points) {
                VertexAttrib3D v{};
                v.pos={point.x,point.y,point.z};
                v.color={1,1,1,1};
                mesh.vertices.push_back(v);
            }
            //注意是先横坐标x后纵坐标y
            mesh.vertices[0].uv={0,0};//左上
            mesh.vertices[1].uv={1,0};//右上
            mesh.vertices[2].uv={1,1};//右下
            mesh.vertices[3].uv={0,1};//左下

            mesh.vertices[4].uv={0,0};//左上
            mesh.vertices[5].uv={1,0};//右上
            mesh.vertices[6].uv={1,1};//右下
            mesh.vertices[7].uv={0,1};//左下
            mesh.indices=shape.indices;
            Material material=Material{"3d_pipeline",{255,255,255,255},""};
            ecs::setComponent<Drawable3DFlag>(this,cube,Drawable3DFlag{0,material,mesh});
        }
        {

            Entity camera2=ecs::createEntity(this);
            ecs::setComponent<ecs::Enabled>(this,camera2, ecs::Enabled{true});
            ecs::setComponent<Transform3DComp>(this,camera2,Transform3DComp{});
            // auto* gpu = ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
            auto* target=new ColorBuffer(800,600);
            ecs::setComponent<Camera3DComp>(this,camera2,{target,glm::radians(60.0f),1.0f/1,-1,-1000});
            auto* renderSystem=new RenderSystem(this);
            ecs::addSystem(this, renderSystem, {});
        }
    }
    ~Scene2() {
        std::cout<<"析构Scene2"<<std::endl;
    }
};
void DemogameApplication::init() {
    //全局数据区

    scene=new Scene2();
}
