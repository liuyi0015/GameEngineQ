//
// Created by XL0002 on 2026/7/16.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H
#include <iostream>
#include <memory>

#include "Context.hpp"
#include "EventDispatcher.h"
#include "ecs/Scene.h"
#include "ecs/System.h"


class EcsApplication {
protected:
    std::shared_ptr<Scene > scene;
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::vector<std::shared_ptr< ecs::System> > global_systems;
public:
    EcsApplication() {
        scene=std::make_shared<Scene>();
        EventDispatcher::getInstance().subscribe("switch_scene",[this](std::any param){
            auto newSceneName=std::any_cast<std::string>(param);
            if (scenes.find(newSceneName) != scenes.end()) {
                scene=scenes[newSceneName];
                for (const auto& gSystem:global_systems) {
                    gSystem->scene=scene;
                }
                start();
            }
        },false,false);
    }
    virtual ~EcsApplication() = default;

    virtual void init() {
    }
    void start() {
        for (const auto& global_system:global_systems) {
            global_system->start();
        }
        for (const auto& system:scene->systems){
            system->start();
        }
    }
    void update(double deltaTime) {
        for (const auto& global_system:global_systems) {
            global_system->update(deltaTime);
        }
        for (const auto& system:scene->systems){
            system->update(deltaTime);
        }
    }
    void fixed_update(double deltaTime) {
        for (const auto& global_system:global_systems) {
            global_system->fixed_update(deltaTime);
        }
        for (const auto& system:scene->systems){
            system->fixed_update(deltaTime);
        }
    }
    void draw() {
        for (const auto& global_system:global_systems) {
            global_system->draw();
        }
        for (const auto& system:scene->systems){
            system->draw();
        }
    }
};

#endif //GAMEENGINE_APPLICATION_H
