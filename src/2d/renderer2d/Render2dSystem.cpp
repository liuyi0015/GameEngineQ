//
// Created by XL0002 on 2026/7/22.
//

#include "Render2dSystem.h"

#include <algorithm>
#include <cassert>

#include "RenderComponents.h"
#include "SDL3/SDL_log.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "../../ResourceManager.hpp"
#include "../transform2d/Transform2dComponents.h"
#include "../transform2d/TransformUtil.h"
#include "../../ecs/Util.h"
#include "cmath"
#include "Render2dUtil.h"
#include "../../Context.hpp"
#include "../../Config.h"
Render2dSystem::~Render2dSystem() {
    //2d场景里通常只有一个渲染系统，不会有其他渲染系统重复释放
    for (const auto entity:ecs::getEntities<ImageRendererFlag>(scene)) {
        auto imageRendererComp = ecs::getComponent<ImageRendererFlag>(scene, entity).value();
        auto texture=ResourceManager::getInstance().getTextureCache().get(imageRendererComp.texResourceId);
        SDL_DestroyTexture(texture);
        ResourceManager::getInstance().getTextureCache().erase(imageRendererComp.texResourceId);
    }
    for (const auto entity:ecs::getEntities<TextRendererFlag>(scene)) {
        auto imageRendererComp = ecs::getComponent<TextRendererFlag>(scene, entity).value();
        auto texture=ResourceManager::getInstance().getTextureCache().get(imageRendererComp.texResourceId);
        SDL_DestroyTexture(texture);
        ResourceManager::getInstance().getTextureCache().erase(imageRendererComp.texResourceId);
    }
}
void Render2dSystem::start() {

    //筛选加载资源
    auto drawableEntities=ecs::getEntities<DrawableFlag>(scene);
    std::cout<<"初始绘制"<<drawableEntities.size()<<"个实体"<<std::endl;
    for (const auto entity:drawableEntities) {
        if (ecs::getComponent<ImageRendererFlag>(scene, entity).has_value()) {
            auto imageRendererComp = ecs::getComponent<ImageRendererFlag>(scene, entity).value();
            if (imageRendererComp.imagePath.empty()) {
                std::cout<<"跳过占位资源初始化texture "<<imageRendererComp.texResourceId<<std::endl;
                continue;
            }
            //加载图片、缓存
            SDL_Surface* surf1=IMG_Load(imageRendererComp.imagePath.c_str());
            std::cout<<surf1->w<<" "<<surf1->h<<std::endl;
            SDL_Texture* tex1=SDL_CreateTextureFromSurface(renderer, surf1);
            ResourceManager::getInstance().getTextureCache().set(imageRendererComp.texResourceId, tex1);
            SDL_DestroySurface(surf1);
        }else if (ecs::getComponent<TextRendererFlag>(scene, entity).has_value()) {
            auto textRendererComp = ecs::getComponent<TextRendererFlag>(scene, entity).value();
            auto font=ResourceManager::getInstance().getFontCache().get(textRendererComp.fontResourceId);
            SDL_Color colorToUse = ecs::getComponent<DrawableFlag>(scene, entity).value().color;
            auto sdlColor = SDL_Color{colorToUse.r, colorToUse.g, colorToUse.b, colorToUse.a};
            SDL_Surface* textSurf = TTF_RenderText_Blended(font, textRendererComp.text.c_str(), textRendererComp.text.length(), sdlColor);
            SDL_Texture* textTex=SDL_CreateTextureFromSurface(renderer, textSurf);
            ResourceManager::getInstance().getTextureCache().set(textRendererComp.texResourceId, textTex);
            SDL_DestroySurface(textSurf);
        }
    }
}

static glm::mat3 getViewMatrix(const Transform &cameraTransform) {
    Transform transform=cameraTransform;
    transform.scale={1.0f,1.0f};//摄像机的scale不影响视图变换
    glm::mat3 viewMatrix=TransformUtil::getReverseTransformToMatrix(transform);
    return viewMatrix;
}

void Render2dSystem::draw() {
    SDL_SetRenderTarget(renderer,target);
    SDL_SetRenderDrawColor(renderer, 0, 100, 100, 0);//test
    SDL_RenderClear(renderer);
    std::vector<std::pair<int,Entity>> drawableEntities;
    for (const auto entity:ecs::getEntities<DrawableFlag>(scene)) {
        if (ecs::getComponent<DrawableFlag>(scene, entity).has_value()) {
            auto zOrder = ecs::getComponent<DrawableFlag>(scene, entity).value().z_order;
            drawableEntities.push_back({zOrder,entity});
        }
    }
    // sort by zOrder (ascending), stable to preserve insertion order for equal z
    std::stable_sort(drawableEntities.begin(), drawableEntities.end(),
                     [](const auto &a, const auto &b){ return a.first < b.first; });

    for (const auto [zOrder,entity]:drawableEntities) {
        auto transformComp = ecs::getComponent<TransformComp>(scene, entity);
        assert(transformComp.has_value());
        auto drawableFlag= ecs::getComponent<DrawableFlag>(scene, entity);
        auto cameraComp = ecs::getComponent<CameraComp>(scene, camera).value();
        auto cameraTransformComp=ecs::getComponent<TransformComp>(scene, camera).value();
        auto cameraWorldTransform=TransformUtil::computeLocalToWorldTransform(cameraTransformComp,scene);
        auto worldTransform = TransformUtil::computeLocalToWorldTransform(transformComp.value(),scene);
        auto modelMatrix=TransformUtil::transformToMatrix(worldTransform);
        auto viewMatrix = getViewMatrix(cameraWorldTransform);
        auto projectMatrix=glm::mat3(1.0f);//没有投影，单位矩阵
        auto mvpMatrix=projectMatrix*viewMatrix*modelMatrix;
        SDL_Color& color=drawableFlag.value().color;

        if (ecs::getComponent<CircleRendererFlag>(scene, entity).has_value()) {
            auto circleRenderFlagComp=ecs::getComponent<CircleRendererFlag>(scene, entity);
            Render2dUtil::drawCircle(renderer, mvpMatrix,  circleRenderFlagComp.value().radius, color, circleRenderFlagComp.value().segments,cameraComp.captureWidth,cameraComp.captureHeight,target->w,target->h);
        }else if (ecs::getComponent<RectRendererFlag>(scene, entity).has_value()){
            auto rectRenderFlagComp = ecs::getComponent<RectRendererFlag>(scene, entity).value();
            float w = rectRenderFlagComp.width;
            float h = rectRenderFlagComp.height;
            // corners in local space (top-left origin)
            glm::vec2 uvs[4] = {
                {0,0},
                {w,0},
                {w,h},
                {0,h}
            };
            Render2dUtil::drawTexture(renderer,nullptr,uvs,color, mvpMatrix,cameraComp.captureWidth,cameraComp.captureHeight,target->w, target->h);
        }else if (ecs::getComponent<TextRendererFlag>(scene, entity).has_value()) {
            auto textRendererComp = ecs::getComponent<TextRendererFlag>(scene, entity);
            auto cachedTexture = ResourceManager::getInstance().getTextureCache().get(textRendererComp.value().texResourceId);
            // 检查文本纹理是否已在缓存中
            if (!cachedTexture) {
                SDL_Log("Warning:缓存里没有 %s", textRendererComp.value().texResourceId.c_str());
                return;
            }
            float w=cachedTexture->w;
            float h=cachedTexture->h;
            // corners in local space (top-left origin)
            glm::vec2 uvs[4] = {
                {0,0},
                {w,0},
                {w,h},
                {0,h}
            };
            Render2dUtil::drawTexture(renderer, cachedTexture,uvs, color, mvpMatrix,cameraComp.captureWidth,cameraComp.captureHeight,target->w,target->h);
        }else if (ecs::getComponent<ImageRendererFlag>(scene, entity).has_value()) {
            auto imageRendererComp = ecs::getComponent<ImageRendererFlag>(scene, entity);
           auto texture = ResourceManager::getInstance().getTextureCache().get(imageRendererComp.value().texResourceId);
            if (!texture) {
                SDL_Log("Warning:缓存里没有 %s", imageRendererComp.value().texResourceId.c_str());
                return;
            }
            // 获取纹理尺寸
            float w = imageRendererComp.value().width;
            float h = imageRendererComp.value().height;
            // corners in local space (top-left origin)
            glm::vec2 uvs[4] = {
                {0,0},
                {w,0},
                {w,h},
                {0,h}
            };
            Render2dUtil::drawTexture(renderer, texture,uvs, color, mvpMatrix, cameraComp.captureWidth,cameraComp.captureHeight,target->w,target->h);
        }

    }
}