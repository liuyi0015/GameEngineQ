//
// Created by XL0002 on 2026/7/17.
//

#include "DemogameApplication.h"

#include "tiny_obj_c.h"
#include "simple-systems/GameComponents.h"
#include "../ecs/BaseComponents.h"
#include "simple-systems/ChangeTransformSystem.h"
#include "../ecs/systems/anim/AnimationSystem.h"
#include "../ecs/systems/soft-render-blueprint/RenderComponents.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "simple-systems/PrintSystem.h"
#include "../core/ResourceManager.hpp"
#include "../ecs/systems/soft-render-blueprint/SoftRenderSystem.h"
#include "../graphics/Shape2DBuilder.h"
#include "../ecs/util/TransformSceneUtil.h"
#include "../graphics/Shape3DBuilder.h"
#include "3d-systems/ChangeTransform3DSystem.h"
#include "../core/AudioPlayer.h"
#include "../ecs/systems/anim/FrameTimeline.h"
#include "../ecs/systems/render-chain/SdlGpuRenderSystem.h"
#include "3d-systems/CameraControlSystem.h"
#include "../ecs/comps/RenderComponents.h"
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
private:
    void loadContent() {
        {
        //image
            Entity img1_entity=ecs::createEntity(this);
            ecs::setComponent<ecs::Name>(this,img1_entity, ecs::Name{"img1"});
            ecs::setComponent<ecs::Enabled>(this,img1_entity, ecs::Enabled{true});
            ecs::setComponent<Transform2DComp>(this,img1_entity, Transform2DComp{{{300,0}}});
            Shape2D rect=Shape2DBuilder::createRect({0,0},{988,852});
            Mesh mesh;
            VertexAttrib2D vs[4];

            vs[0].pos={rect.points[0].x,rect.points[0].y};
            vs[0].color={1,1,1,1};
            //注意是先横坐标x后纵坐标y
            vs[0].uv={0,0};//左上
            mesh.vertices.push_back(vs[0]);

            vs[1].pos={rect.points[1].x,rect.points[1].y};
            vs[1].color={1,1,1,1};
            vs[1].uv={1,0};//右上
            mesh.vertices.push_back(vs[1]);

            vs[2].pos={rect.points[2].x,rect.points[2].y};
            vs[2].color={1,1,1,1};
            vs[2].uv={1,1};//右下
            mesh.vertices.push_back(vs[2]);

            vs[3].pos={rect.points[3].x,rect.points[3].y};
            vs[3].color={1,1,1,1};
            vs[3].uv={0,1};//左下
            mesh.vertices.push_back(vs[3]);
            mesh.indices=rect.indices;
            auto* img1_surface=ResourceManager::getInstance().getSurfaceCache().get("img1-tex");
            //todo 目前pipeline写死在render2dProcess，这里配置无效
            SoftRecipe recipe={"img1-pipeline",{255,255,255,255},img1_surface};
            Material material;
            material.passRecipeMap["2dpass"] = recipe;
            ecs::setComponent<Drawable2DFlag>(this,img1_entity,Drawable2DFlag{0,material,mesh});

            // ecs::setComponent<RotationFlag>(this,img1_entity,{-40.0f});
            AnimationFlag animFlag;
            AnimationClip animClip;
            animClip.duration=1;
            animClip.loop=true;
            //拿组件里的地址而不是源数据地址，注意取地址符
            SoftRecipe * materialPtrOnDrawable = std::any_cast<SoftRecipe>(
                &ecs::getComponent<Drawable2DFlag>(this,img1_entity)->material.passRecipeMap["2dpass"]);
            auto* frame_timeline=new FrameTimeline("anim1",&materialPtrOnDrawable->surface);
            animClip.timelines.push_back(std::shared_ptr<FrameTimeline>(frame_timeline));
            animFlag.anims.push_back(animClip);
            ecs::setComponent<AnimationFlag>(this,img1_entity,animFlag);
        }
        {
            //render
            //摄像机的视口大小与transform(scale)无关，只看CameraComp？
            Entity camera1=ecs::createEntity(this);
            ecs::setComponent<ecs::Enabled>(this,camera1, ecs::Enabled{true});
            ecs::setComponent<Transform2DComp>(this,camera1,Transform2DComp{});
            ecs::setComponent<Camera2DComp>(this,camera1,{1200,1080});

            // ecs::setComponent<CameraInputListenerFlag>(this, camera1, {});
            // auto* cameraInputListenerSystem=new CameraInputListenerSystem(this);
            // ecs::addSystem(this,cameraInputListenerSystem,{});

        }
    }
    void loadResources() {
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
public:
    Scene1(PrintSystem* printSystem1,PrintSystem* printSystem2,ChangeTransformSystem* changeTransformSystem,
        AnimationSystem* animationSystem,SoftRenderSystem* renderSystem) {
            {

            //普通system
            ecs::addSystem(this,changeTransformSystem,{});
            //顺序test
            ecs::addSystem(this,printSystem1,{10,0,0,0});
            ecs::addSystem(this,printSystem2,{});
            ecs::addSystem(this, renderSystem, {});
            ecs::addSystem(this,animationSystem,{});
            }
        loadResources();
        loadContent();

    }
    ~Scene1() {
        std::cout<<"析构Scene1"<<std::endl;
    }
};

class Scene2:public ecs::Scene {
private:
    void loadResources() {
        auto* img1=IMG_Load("assets/1.png");
            ResourceManager::getInstance().getSurfaceCache().set("img1-tex",img1);
    }
    void loadContent(CameraControlSystem* cameraControlSystem) {
        {
            Entity cube=ecs::createEntity(this);
            ecs::setComponent<ecs::Enabled>(this,cube, ecs::Enabled{true});
            ecs::setComponent<Transform3DComp>(this,cube,Transform3DComp{{{0,0,-5},
                {30,45,30},{100,100,100}}});
            Shape3D shape=Shape3DBuilder::createCube();
            Mesh mesh;
            VertexAttrib3D vs[8];

                vs[0].pos={shape.points[0].x,shape.points[0].y,shape.points[0].z};
                vs[0].color={1,1,1,1};
            //注意是先横坐标x后纵坐标y
            vs[0].uv={0,0};//左上
                mesh.vertices.push_back(vs[0]);

            vs[1].pos={shape.points[1].x,shape.points[1].y,shape.points[1].z};
            vs[1].color={1,1,1,1};
            vs[1].uv={1,0};//右上
            mesh.vertices.push_back(vs[1]);

            vs[2].pos={shape.points[2].x,shape.points[2].y,shape.points[2].z};
            vs[2].color={1,1,1,1};
            vs[2].uv={1,1};//右下
            mesh.vertices.push_back(vs[2]);

            vs[3].pos={shape.points[3].x,shape.points[3].y,shape.points[3].z};
            vs[3].color={1,1,1,1};
            vs[3].uv={0,1};//左下
            mesh.vertices.push_back(vs[3]);

            vs[4].pos={shape.points[4].x,shape.points[4].y,shape.points[4].z};
            vs[4].color={1,1,1,1};
            vs[4].uv={0,0};//左上
            mesh.vertices.push_back(vs[4]);

            vs[5].pos={shape.points[5].x,shape.points[5].y,shape.points[5].z};
            vs[5].color={1,1,1,1};
            vs[5].uv={1,0};//右上
            mesh.vertices.push_back(vs[5]);

            vs[6].pos={shape.points[6].x,shape.points[6].y,shape.points[6].z};
            vs[6].color={1,1,1,1};
            vs[6].uv={1,1};//右下
            mesh.vertices.push_back(vs[6]);

            vs[7].pos={shape.points[7].x,shape.points[7].y,shape.points[7].z};
            vs[7].color={1,1,1,1};
            vs[7].uv={0,1};//左下
            mesh.vertices.push_back(vs[7]);
            mesh.indices=shape.indices;
            SoftRecipe recipe=SoftRecipe{"3d_pipeline",{255,255,255,255},nullptr};
            Material material;
            material.passRecipeMap["3dpass"] = recipe;
            ecs::setComponent<Drawable3DFlag>(this,cube,Drawable3DFlag{material,mesh});
        }
        {
            Entity camera2=ecs::createEntity(this);
            ecs::setComponent<ecs::Enabled>(this,camera2, ecs::Enabled{true});
            ecs::setComponent<Transform3DComp>(this,camera2,Transform3DComp{{{},{0,180,0}}});
            ecs::setComponent<Rotation3DFlag>(this,camera2,Rotation3DFlag{10,10,10});
            ecs::setComponent<Camera3DComp>(this,camera2,{glm::radians(60.0f),1.0f/1,-1,-1000});

            cameraControlSystem->camera=camera2;
        }
    }
public:
    //gpu渲染

    Scene2(ChangeTransform3DSystem* changeTransform3dSystem,SdlGpuRenderSystem* renderSystem,CameraControlSystem* cameraControlSystem){
        {
            // ecs::addSystem(this, changeTransform3dSystem, {});
            ecs::addSystem(this, renderSystem, {});
            ecs::addSystem(this, cameraControlSystem, {});
        }
        loadResources();
        loadContent(cameraControlSystem);

    }
    //软渲染
    Scene2(ChangeTransform3DSystem* changeTransform3dSystem,SoftRenderSystem* renderSystem,CameraControlSystem* cameraControlSystem) {
        {
            // ecs::addSystem(this, changeTransform3dSystem, {});
            ecs::addSystem(this, renderSystem, {});
            ecs::addSystem(this, cameraControlSystem, {});
        }
        loadResources();
        loadContent(cameraControlSystem);
    }
    ~Scene2() {
        std::cout<<"析构Scene2"<<std::endl;
    }
};


void DemogameApplication::init() {
    //全局数据区
    //system可能要多实例，所以不能静态
    auto* print_system1 = new PrintSystem(1);
    auto* print_system2 = new PrintSystem(2);
    auto* changeTransformSystem=new ChangeTransformSystem();
    auto* animationSystem=new AnimationSystem();
    auto* softRenderSystem=new SoftRenderSystem();
    auto* sdlgpuRenderSystem=new SdlGpuRenderSystem();
    auto* changeTransform3dSystem=new ChangeTransform3DSystem();
    auto* cameraControlSystem=new CameraControlSystem();
    ecs::Scene* scene1=new Scene1(print_system1,print_system2,changeTransformSystem,animationSystem,softRenderSystem);
    ecs::Scene* scene2=new Scene2(changeTransform3dSystem,sdlgpuRenderSystem,cameraControlSystem);
    scene=scene2;
	AudioPlayer::loadAndPlay("assets/2.mp3");
    scene->init();
}
