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

void Render2dSystem::start() {

    //加载资源
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

    std::cout<<"绘制"<<drawableEntities.size()<<"个实体"<<std::endl;
    for (const auto [zOrder,entity]:drawableEntities) {
        if (ecs::getComponent<ImageRendererFlag>(scene, entity).has_value()) {
            //加载图片、缓存
            SDL_Surface* surf1=IMG_Load("assets/1.png");
            std::cout<<surf1->w<<" "<<surf1->h<<std::endl;
            SDL_Texture* tex1=SDL_CreateTextureFromSurface(renderer, surf1);
            ResourceManager::getInstance().getSurfaceCache().set("img1-surf", std::shared_ptr<SDL_Surface>(surf1, SDL_DestroySurface));
            ResourceManager::getInstance().getTextureCache().set("img1-tex", std::shared_ptr<SDL_Texture>(tex1, SDL_DestroyTexture));
        }else if (ecs::getComponent<TextRendererFlag>(scene, entity).has_value()) {
            auto font=ResourceManager::getInstance().getFontCache().get("font1");
            const std::string s=ecs::getComponent<TextRendererFlag>(scene, entity).value().text;
            Color colorToUse = ecs::getComponent<DrawableFlag>(scene, entity).value().color;
            auto sdlColor = SDL_Color{colorToUse.r, colorToUse.g, colorToUse.b, colorToUse.a};
            SDL_Surface* textSurf = TTF_RenderText_Blended(font.get(), s.c_str(), s.length(), sdlColor);
            auto textRendererComp = ecs::getComponent<TextRendererFlag>(scene, entity);
            ResourceManager::getInstance().getSurfaceCache().set(textRendererComp.value().surfResourceId, std::shared_ptr<SDL_Surface>(textSurf, SDL_DestroySurface));
            SDL_Texture* textTex=SDL_CreateTextureFromSurface(renderer, textSurf);
            ResourceManager::getInstance().getTextureCache().set(textRendererComp.value().texResourceId, std::shared_ptr<SDL_Texture>(textTex, SDL_DestroyTexture));
        }
    }
}

static void drawCircle(SDL_Renderer *renderer, float x, float y, float radius, const Color &color, int segments) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // 使用多个线段近似圆形

    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        float x1 = x + std::cos(angle1) * radius;
        float y1 = y + std::sin(angle1) * radius;
        float x2 = x + std::cos(angle2) * radius;
        float y2 = y + std::sin(angle2) * radius;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

static void drawImage(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect dstRect, const Color color,float angle) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FPoint center = { 0,0 };
    SDL_RenderTextureRotated(renderer, texture, nullptr, &dstRect, angle, &center, SDL_FLIP_NONE);
}
static void drawRect(SDL_Renderer *renderer, const SDL_FRect rect, const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer,&rect);
}
static void drawText(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect dstRect) {
    SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
}
void Render2dSystem::draw() {
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
        auto transformComp = ecs::getComponent<Transform>(scene, entity);
        assert(transformComp.has_value());
        auto drawableFlag= ecs::getComponent<DrawableFlag>(scene, entity);
        auto worldTransform = TransformUtil::computeLocalToWorldTransform(transformComp.value(),scene);
        auto viewTransform = TransformUtil::computeWorldToViewTransform(worldTransform,scene,camera);
        Color& color=drawableFlag.value().color;

        if (ecs::getComponent<CircleRendererFlag>(scene, entity).has_value()) {
            auto circleRenderFlagComp=ecs::getComponent<CircleRendererFlag>(scene, entity);

            // 使用多个线段近似圆形
            int segments = circleRenderFlagComp.value().segments;
            float radius = circleRenderFlagComp.value().radius*viewTransform.scale.x;
            float x=viewTransform.position.x;
            float y=viewTransform.position.y;
            drawCircle(renderer, x, y, radius, color, segments);
        }else if (ecs::getComponent<RectRendererFlag>(scene, entity).has_value()) {
            auto rectRenderFlagComp = ecs::getComponent<RectRendererFlag>(scene, entity);
            float width = rectRenderFlagComp.value().width;
            float height = rectRenderFlagComp.value().height;
            SDL_FRect rect = {viewTransform.position.x, viewTransform.position.y, width * viewTransform.scale.x, height * viewTransform.scale.y};
            drawRect(renderer, rect, color);
        }else if (ecs::getComponent<TextRendererFlag>(scene, entity).has_value()) {
            auto textRendererComp = ecs::getComponent<TextRendererFlag>(scene, entity);
            auto cachedTexture = ResourceManager::getInstance().getTextureCache().get(textRendererComp.value().texResourceId);

            // 检查文本纹理是否已在缓存中
            if (!cachedTexture) {
                SDL_Log("Warning:缓存里没有");
                return;
            }

            // 渲染文本纹理
            SDL_FRect dstRect = {
                viewTransform.position.x,
                viewTransform.position.y,
                300 * viewTransform.scale.x,
                100 * viewTransform.scale.y
            };

            drawText(renderer, cachedTexture.get(), dstRect);

        }else if (ecs::getComponent<ImageRendererFlag>(scene, entity).has_value()) {
            auto imageRendererComp = ecs::getComponent<ImageRendererFlag>(scene, entity);
           auto texture = ResourceManager::getInstance().getTextureCache().get(imageRendererComp.value().textureId);

            if (!texture) {
                SDL_Log("Warning:texture resource '%s' not found in ResourceManager", imageRendererComp.value().textureId.c_str());
                return;
            }

            // 获取纹理尺寸
            float width = imageRendererComp.value().width;
            float height = imageRendererComp.value().height;

            SDL_FRect dstRect = {
                viewTransform.position.x,
                viewTransform.position.y,
                width * viewTransform.scale.x,
                height * viewTransform.scale.y
            };
            drawImage(renderer, texture.get(), dstRect, color,viewTransform.rotation.angle);

        }

    }
}
