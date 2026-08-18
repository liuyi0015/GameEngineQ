//
// Created by abc17 on 2026/8/12.
//

#ifndef GAMEENGINE_RENDERCOMPOSITOR_H
#define GAMEENGINE_RENDERCOMPOSITOR_H
#include <vector>

#include "Context.hpp"
#include "ecs/Scene.h"
#include "SDL3/SDL_render.h"


class RenderCompositor {
public:
    std::unordered_map<ecs::Scene*, size_t>sceneIndices;
    std::vector<SDL_Texture*> targets;
    void addTarget(ecs::Scene* scene,SDL_Texture* target) {
        targets.push_back(target);
        const size_t idx=sceneIndices.size()-1;
        sceneIndices[scene]=idx;
    }
    void removeTarget(ecs::Scene* scene) {
        const size_t idx=sceneIndices[scene];
        //结构体没有析构函数，只能用销毁方法
        SDL_DestroyTexture( targets[idx]);
        targets.erase(targets.begin()+static_cast<long long>(idx));
    }
    void compositeTextures() {
        auto* renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        SDL_SetRenderTarget(renderer,nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 100, 100, 255);//test
        //清屏
        SDL_RenderClear(renderer);
        for (auto target:targets) {
            SDL_RenderTexture(renderer,target,nullptr,nullptr);
        }
    }
};


#endif //GAMEENGINE_RENDERCOMPOSITOR_H
