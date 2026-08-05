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
            SDL_Color colorToUse = ecs::getComponent<DrawableFlag>(scene, entity).value().color;
            auto sdlColor = SDL_Color{colorToUse.r, colorToUse.g, colorToUse.b, colorToUse.a};
            SDL_Surface* textSurf = TTF_RenderText_Blended(font.get(), s.c_str(), s.length(), sdlColor);
            auto textRendererComp = ecs::getComponent<TextRendererFlag>(scene, entity);
            ResourceManager::getInstance().getSurfaceCache().set(textRendererComp.value().surfResourceId, std::shared_ptr<SDL_Surface>(textSurf, SDL_DestroySurface));
            SDL_Texture* textTex=SDL_CreateTextureFromSurface(renderer, textSurf);
            ResourceManager::getInstance().getTextureCache().set(textRendererComp.value().texResourceId, std::shared_ptr<SDL_Texture>(textTex, SDL_DestroyTexture));
        }
    }
}
static SDL_FPoint vertexShader(const glm::mat3 &mat, float lx, float ly) {
    glm::vec3 p = mat * glm::vec3(lx, ly, 1.0f);
    return SDL_FPoint{ p.x, p.y };
}
static void drawCircle(SDL_Renderer* renderer, const glm::mat3 &mat, float radius, const SDL_Color &color, int segments) {
    if (segments < 3) segments = 3;
    const int numVerts = segments + 1;
    std::vector<SDL_Vertex> verts;
    verts.reserve(numVerts);
    // center at local (0,0)

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float lx = std::cos(angle) * radius;
        float ly = std::sin(angle) * radius;
        SDL_Vertex v;
        v.position = vertexShader(mat, lx, ly);
        v.color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
        v.tex_coord = SDL_FPoint{ 0.0f, 0.0f };
        verts.push_back(v);
    }
    std::vector<int> indices;
    indices.reserve(segments * 3);
    for (int i = 0; i < segments; ++i) {
        int a = 0;
        int b = i + 1;
        int c = (i + 2 <= segments) ? (i + 2) : 1;
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
    }
    SDL_RenderGeometry(renderer, nullptr, verts.data(), (int)verts.size(), indices.data(), (int)indices.size());
}
static void drawCircle(SDL_Renderer *renderer, float x, float y, float radius, const SDL_Color &color, int segments) {
    if (segments < 3) segments = 3;
    const int numVerts = segments + 1; // center + ring
    std::vector<SDL_Vertex> verts;
    verts.reserve(numVerts);
    // center vertex
    SDL_Vertex centerV;
    centerV.position = SDL_FPoint{ x, y };
    centerV.color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
    centerV.tex_coord = SDL_FPoint{ 0.0f, 0.0f };
    verts.push_back(centerV);

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float vx = x + std::cos(angle) * radius;
        float vy = y + std::sin(angle) * radius;
        SDL_Vertex v;
        v.position = SDL_FPoint{ vx, vy };
        v.color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
        v.tex_coord = SDL_FPoint{ 0.0f, 0.0f };
        verts.push_back(v);
    }

    std::vector<int> indices;
    indices.reserve(segments * 3);
    for (int i = 0; i < segments; ++i) {
        int a = 0;
        int b = i + 1;
        int c = (i + 2 <= segments) ? (i + 2) : 1;
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
    }

    SDL_RenderGeometry(renderer, nullptr, verts.data(), (int)verts.size(), indices.data(), (int)indices.size());
}
static void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, float w, float h, const SDL_Color color, const Transform transform) {
    // corners in local space (top-left origin)
    glm::vec2 corners[4] = {
        { transform.position.x,        transform.position.y },
        { transform.position.x + w,    transform.position.y },
        { transform.position.x + w,    transform.position.y + h },
        { transform.position.x,        transform.position.y + h }
    };
    SDL_Vertex verts[4];
    for (int i = 0; i < 4; ++i) {
        verts[i].position = vertexShader(TransformUtil::transformToMatrix(transform), corners[i].x, corners[i].y);
        verts[i].color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
    }
    verts[0].tex_coord = SDL_FPoint{ 0.0f, 0.0f };
    verts[1].tex_coord = SDL_FPoint{ 1.0f, 0.0f };
    verts[2].tex_coord = SDL_FPoint{ 1.0f, 1.0f };
    verts[3].tex_coord = SDL_FPoint{ 0.0f, 1.0f };
    int indices[6] = { 0, 1, 2, 2, 3, 0 };
    SDL_RenderGeometry(renderer, texture, verts, 4, indices, 6);
}
static void drawTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect dstRect, const SDL_Color color, float angleDegrees) {
    // Build textured quad and rotate around top-left (to match previous behavior where center was {0,0})
    float x = dstRect.x;
    float y = dstRect.y;
    float w = dstRect.w;
    float h = dstRect.h;

    float rad = angleDegrees * (M_PI / 180.0f);
    float c = std::cos(rad);
    float s = std::sin(rad);

    SDL_FPoint corners[4] = {
        { 0.0f, 0.0f },
        { w, 0.0f },
        { w, h },
        { 0.0f, h }
    };

    SDL_Vertex verts[4];
    for (int i = 0; i < 4; ++i) {
        float dx = corners[i].x;
        float dy = corners[i].y;
        float rx = x + (dx * c - dy * s);
        float ry = y + (dx * s + dy * c);
        verts[i].position = SDL_FPoint{ rx, ry };
        verts[i].color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
    }
    // texture coordinates: (0,0),(1,0),(1,1),(0,1)
    verts[0].tex_coord = SDL_FPoint{ 0.0f, 0.0f };
    verts[1].tex_coord = SDL_FPoint{ 1.0f, 0.0f };
    verts[2].tex_coord = SDL_FPoint{ 1.0f, 1.0f };
    verts[3].tex_coord = SDL_FPoint{ 0.0f, 1.0f };

    int indices[6] = { 0, 1, 2, 2, 3, 0 };
    SDL_RenderGeometry(renderer, texture, verts, 4, indices, 6);
}

Transform computeWorldToViewTransform(const Transform &worldTransform,const std::shared_ptr<Scene> &scene,const Entity camera) {
    auto cameraTransform=ecs::getComponent<TransformComp>(scene, camera).value();
    auto cameraWorldTransform=TransformUtil::computeLocalToWorldTransform(cameraTransform,scene);
    cameraWorldTransform.scale={1.0f,1.0f};//摄像机的scale不影响视图变换
    Transform viewTransform = TransformUtil::computeRelativeTransform(cameraTransform, {worldTransform,-1}, scene);
    return viewTransform;
}
Position computeViewportToTargetPosition(const Transform &viewTransform,const std::shared_ptr<Scene> &scene,const Entity camera,int targetWidth,int targetHeight) {
    //先转换到[-1,1]^2 ndc空间
    CameraComp cameraComp=ecs::getComponent<CameraComp>(scene, camera).value();
    Position ndc_position;
    ndc_position.x=viewTransform.position.x*viewTransform.scale.x/cameraComp.viewportWidth*2.0f-1.0f;
    ndc_position.y=viewTransform.position.y*viewTransform.scale.y/cameraComp.viewportHeight*2.0f-1.0f;
    //再转换到屏幕空间
    Position screenPosition;
    screenPosition.x=ndc_position.x*targetWidth/2.0f+targetWidth/2.0f;
    screenPosition.y=ndc_position.y*targetHeight/2.0f+targetHeight/2.0f;
    return screenPosition;
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
        auto transformComp = ecs::getComponent<TransformComp>(scene, entity);
        assert(transformComp.has_value());
        auto drawableFlag= ecs::getComponent<DrawableFlag>(scene, entity);
        auto worldTransform = TransformUtil::computeLocalToWorldTransform(transformComp.value(),scene);
        auto viewTransform = computeWorldToViewTransform(worldTransform,scene,camera);
        SDL_Color& color=drawableFlag.value().color;

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
            drawTexture(renderer,nullptr, rect, color, viewTransform.rotation.angle);
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

            drawTexture(renderer, cachedTexture.get(), dstRect,color,viewTransform.rotation.angle);

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
            drawTexture(renderer, texture.get(), dstRect, color,viewTransform.rotation.angle);

        }

    }
}
