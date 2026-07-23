//
// Created by XL0002 on 2026/7/22.
//

#include "RenderSystem.h"

#include <algorithm>
#include <cassert>
#include <map>
#include "../ecs/BaseComponents.h"

#include "RenderComponents.h"
#include "RenderUtil.h"
#include "../ecs/Scene.h"
#include "../ecs/TransformUtil.h"
#include "SDL3/SDL_log.h"
#include "../ResourceManager.hpp"
#include "SDL3_image/SDL_image.h"

void RenderSystem::start() {
    std::vector<std::pair<int,Entity>> drawableEntities;
    for (const auto entity:scene->getEntities()) {
        if (scene->getComponent<DrawableFlag>(entity).has_value()) {
            auto zOrder = scene->getComponent<DrawableFlag>(entity).value().z_order;
            drawableEntities.push_back({zOrder,entity});
        }
    }
    // sort by zOrder (ascending), stable to preserve insertion order for equal z
    std::stable_sort(drawableEntities.begin(), drawableEntities.end(),
                     [](const auto &a, const auto &b){ return a.first < b.first; });

    std::cout<<"绘制"<<drawableEntities.size()<<"个实体"<<std::endl;
    for (const auto [zOrder,entity]:drawableEntities) {
        if (scene->getComponent<ImageRendererFlag>(entity).has_value()) {
            //加载图片、缓存
            SDL_Surface* surf1=IMG_Load("assets/1.png");
            std::cout<<surf1->w<<" "<<surf1->h<<std::endl;
            SDL_Texture* tex1=SDL_CreateTextureFromSurface(renderer, surf1);
            ResourceManager::getInstance().getSurfaceCache().set("img1-surf", std::shared_ptr<SDL_Surface>(surf1, SDL_DestroySurface));
            ResourceManager::getInstance().getTextureCache().set("img1-tex", std::shared_ptr<SDL_Texture>(tex1, SDL_DestroyTexture));
        }else if (scene->getComponent<TextRendererFlag>(entity).has_value()) {
            auto font=ResourceManager::getInstance().getFontCache().get("font1");
            const std::string s=scene->getComponent<TextRendererFlag>(entity).value().text;
            Color colorToUse = scene->getComponent<DrawableFlag>(entity).value().color;
            auto sdlColor = SDL_Color{colorToUse.r, colorToUse.g, colorToUse.b, colorToUse.a};
            SDL_Surface* textSurf = TTF_RenderText_Blended(font.get(), s.c_str(), s.length(), sdlColor);
            auto textRendererComp = scene->getComponent<TextRendererFlag>(entity);
            ResourceManager::getInstance().getSurfaceCache().set(textRendererComp.value().surfResourceId, std::shared_ptr<SDL_Surface>(textSurf, SDL_DestroySurface));
            SDL_Texture* textTex=SDL_CreateTextureFromSurface(renderer, textSurf);
            ResourceManager::getInstance().getTextureCache().set(textRendererComp.value().texResourceId, std::shared_ptr<SDL_Texture>(textTex, SDL_DestroyTexture));
        }
    }
}
void RenderSystem::draw() {
    std::vector<std::pair<int,Entity>> drawableEntities;
    for (const auto entity:scene->getEntities()) {
        if (scene->getComponent<DrawableFlag>(entity).has_value()) {
            auto zOrder = scene->getComponent<DrawableFlag>(entity).value().z_order;
            drawableEntities.push_back({zOrder,entity});
        }
    }
    // sort by zOrder (ascending), stable to preserve insertion order for equal z
    std::stable_sort(drawableEntities.begin(), drawableEntities.end(),
                     [](const auto &a, const auto &b){ return a.first < b.first; });

    for (const auto [zOrder,entity]:drawableEntities) {
        auto transformComp = scene->getComponent<ecs::Transform>(entity);
        assert(transformComp.has_value());
        auto drawableFlag= scene->getComponent<DrawableFlag>(entity);
        auto worldTransform = TransformUtil::computeWorldTransform(transformComp.value(),scene);
        Color& color=drawableFlag.value().color;

        if (scene->getComponent<CircleRendererFlag>(entity).has_value()) {
            auto circleRenderFlagComp=scene->getComponent<CircleRendererFlag>(entity);

            // 使用多个线段近似圆形
            int segments = circleRenderFlagComp.value().segments;
            float radius = circleRenderFlagComp.value().radius;
            float x=worldTransform.position.x;
            float y=worldTransform.position.y;
            RenderUtil::drawCircle(renderer, x, y, radius, color, segments);
        }else if (scene->getComponent<RectRendererFlag>(entity).has_value()) {
            auto rectRenderFlagComp = scene->getComponent<RectRendererFlag>(entity);
            float width = rectRenderFlagComp.value().width;
            float height = rectRenderFlagComp.value().height;
            SDL_FRect rect = {worldTransform.position.x, worldTransform.position.y, width * worldTransform.scale.x, height * worldTransform.scale.y};
            RenderUtil::drawRect(renderer, rect, color);
        }else if (scene->getComponent<TextRendererFlag>(entity).has_value()) {
            auto textRendererComp = scene->getComponent<TextRendererFlag>(entity);
            auto cachedTexture = ResourceManager::getInstance().getTextureCache().get(textRendererComp.value().texResourceId);

            // 检查文本纹理是否已在缓存中
            if (!cachedTexture) {
                SDL_Log("Warning:缓存里没有");
                return;
            }

            // 渲染文本纹理
            SDL_FRect dstRect = {
                worldTransform.position.x,
                worldTransform.position.y,
                300 * worldTransform.scale.x,
                100 * worldTransform.scale.y
            };

            RenderUtil::drawText(renderer, cachedTexture.get(), dstRect, color);

        }else if (scene->getComponent<ImageRendererFlag>(entity).has_value()) {
            auto imageRendererComp = scene->getComponent<ImageRendererFlag>(entity);
           auto texture = ResourceManager::getInstance().getTextureCache().get(imageRendererComp.value().resourceId);

            if (!texture) {
                SDL_Log("Warning: Texture resource '%s' not found in ResourceManager", imageRendererComp.value().resourceId.c_str());
                return;
            }

            // 获取纹理尺寸
            float width = imageRendererComp.value().width;
            float height = imageRendererComp.value().height;

            SDL_FRect dstRect = {
                worldTransform.position.x,
                worldTransform.position.y,
                width * worldTransform.scale.x,
                height * worldTransform.scale.y
            };
            RenderUtil::drawImage(renderer, texture.get(), dstRect, color, worldTransform.rotation.angle);

        }

    }
}
