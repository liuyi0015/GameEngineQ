//
// Created by XL0002 on 2026/7/16.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H
#include <algorithm>
#include <iostream>
#include <memory>

#include "Context.hpp"
#include "EventDispatcher.h"
#include "ecs/Scene.h"
#include "ecs/System.h"
#include "ecs/Util.h"
#include "SDL3/SDL_log.h"


struct SystemComparer{
    bool operator()(const SystemOrder& a, const SystemOrder& b) const {
        return a.second < b.second;
    }
};
//可以直接用在main里，相当于空应用
class EcsApplication {
private:
protected:
    Scene* scene;
    std::vector<Scene*> global_scenes;
    void switch_scene(const std::string& sceneName) {
        auto* oldScene=scene;
        scene=loadScene(sceneName);
        //手动释放
        delete oldScene;
        if (scene!=nullptr) {
            //修改全局参数
            for (const auto& gScene:global_scenes) {
                for (const auto& [systemId,system]:gScene->systemIds) {
                    system->scene=scene;
                }
            }
            //
            std::sort(scene->system_start_orders.begin(), scene->system_start_orders.end(), SystemComparer());
            std::sort(scene->system_update_orders.begin(), scene->system_update_orders.end(), SystemComparer());
            std::sort(scene->system_fixed_update_orders.begin(), scene->system_fixed_update_orders.end(), SystemComparer());
            std::sort(scene->system_draw_orders.begin(), scene->system_draw_orders.end(), SystemComparer());
            start();
        }else {
            SDL_Log("场景%s不存在",sceneName.c_str());
        }
    }
public:
    EcsApplication() {
        //默认保留一个空场景，不然空指针会导致程序崩溃
        scene=new Scene();
    }
    virtual ~EcsApplication() = default;
    virtual Scene* loadScene(std::string sceneName) { return nullptr; };
    virtual void init() {
        EventDispatcher::getInstance().subscribe("switch_scene",[this](std::any param) {
            const auto sceneName=std::any_cast<std::string>(param);
            switch_scene(sceneName);
        },false,false);
        for (const auto& scene : global_scenes) {
            std::sort(scene->system_start_orders.begin(), scene->system_start_orders.end(), SystemComparer());
            std::sort(scene->system_update_orders.begin(), scene->system_update_orders.end(), SystemComparer());
            std::sort(scene->system_fixed_update_orders.begin(), scene->system_fixed_update_orders.end(), SystemComparer());
            std::sort(scene->system_draw_orders.begin(), scene->system_draw_orders.end(), SystemComparer());
        }
    }
    void start() {
        for (const auto& scene:global_scenes) {
            for (const auto& systemOrder:scene->system_start_orders) {
                auto system=ecs::getSystemById(scene,systemOrder.first);
                system->start();
            }
        }
        for (const auto& systemOrder:scene->system_start_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->start();
        }
    }
    void update(double deltaTime) {
        for (const auto& scene:global_scenes) {
            for (const auto& systemOrder:scene->system_update_orders) {
                auto system=ecs::getSystemById(scene,systemOrder.first);
                system->update(deltaTime);
            }
        }
        for (const auto& systemOrder:scene->system_update_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->update(deltaTime);
        }
    }
    void fixed_update(double deltaTime) {
        for (const auto& scene:global_scenes) {
            for (const auto& systemOrder:scene->system_fixed_update_orders) {
                auto system=ecs::getSystemById(scene,systemOrder.first);
                system->fixed_update(deltaTime);
            }
        }
        for (const auto& systemOrder:scene->system_fixed_update_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->fixed_update(deltaTime);
        }
    }
    void draw() {
        for (const auto& scene:global_scenes) {
            for (const auto& systemOrder:scene->system_draw_orders) {
                auto system=ecs::getSystemById(scene,systemOrder.first);
                system->draw();
            }
        }
        for (const auto& systemOrder:scene->system_draw_orders) {
            auto system=ecs::getSystemById(scene,systemOrder.first);
            system->draw();
        }
    }
};

#endif //GAMEENGINE_APPLICATION_H
