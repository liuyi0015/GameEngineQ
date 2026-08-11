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
#include "../../Context.hpp"
#include "../../Config.h"

void Render2dSystem::start() {

    //筛选加载资源
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
            ResourceManager::getInstance().getSurfaceCache().set("img1-surf", surf1);
            ResourceManager::getInstance().getTextureCache().set("img1-tex", tex1);
        }else if (ecs::getComponent<TextRendererFlag>(scene, entity).has_value()) {
            auto font=ResourceManager::getInstance().getFontCache().get("font1");
            const std::string s=ecs::getComponent<TextRendererFlag>(scene, entity).value().text;
            SDL_Color colorToUse = ecs::getComponent<DrawableFlag>(scene, entity).value().color;
            auto sdlColor = SDL_Color{colorToUse.r, colorToUse.g, colorToUse.b, colorToUse.a};
            SDL_Surface* textSurf = TTF_RenderText_Blended(font, s.c_str(), s.length(), sdlColor);
            auto textRendererComp = ecs::getComponent<TextRendererFlag>(scene, entity);
            ResourceManager::getInstance().getSurfaceCache().set(textRendererComp.value().surfResourceId, textSurf);
            SDL_Texture* textTex=SDL_CreateTextureFromSurface(renderer, textSurf);
            ResourceManager::getInstance().getTextureCache().set(textRendererComp.value().texResourceId, textTex);
        }
    }
}
//模型空间变为裁剪空间
static SDL_FPoint vertexShader(const glm::mat3 &mvpMatrix, float lx, float ly) {
    glm::vec3 p = mvpMatrix * glm::vec3(lx, ly, 1.0f);
    return SDL_FPoint{ p.x, p.y };
}

static glm::mat3 getViewMatrix(const Transform &cameraTransform) {
    Transform transform=cameraTransform;
    transform.scale={1.0f,1.0f};//摄像机的scale不影响视图变换
    glm::mat3 viewMatrix=TransformUtil::getReverseTransformToMatrix(transform);
    return viewMatrix;
}
// 使用多个线段近似圆形
static void drawCircle(SDL_Renderer* renderer, const glm::mat3 mvpMatrix, float radius, const SDL_Color &color, int segments,float clipWidth, float clipHeight) {
    if (segments < 3) segments = 3;
    const int numVerts = segments + 1;
    std::vector<SDL_Vertex> verts;
    verts.reserve(numVerts);
    // center at local (0,0)
    auto config=ApplicationContext::getInstance().get<Config>("config");
    // 圆心顶点（索引 0）
    SDL_Vertex centerV{};
    SDL_FPoint pos = vertexShader(mvpMatrix, 0.0f, 0.0f);
    centerV.position.x = (pos.x / clipWidth * 0.5f + 0.5f) * config.LOGIC_WIDTH;
    centerV.position.y = (pos.y / clipHeight * 0.5f + 0.5f) * config.LOGIC_HEIGHT;
    centerV.color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
    centerV.tex_coord = SDL_FPoint{0.0f, 0.0f};
    verts.push_back(centerV);
    // 使用多个线段近似圆形
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float lx = std::cos(angle) * radius;
        float ly = std::sin(angle) * radius;
        SDL_Vertex v;
        SDL_FPoint pos = vertexShader(mvpMatrix, lx,ly);
        //裁剪空间->ndc->视口
        v.position.x=(pos.x/clipWidth*0.5f+0.5f)*config.LOGIC_WIDTH;
        v.position.y=(pos.y/clipHeight*0.5f+0.5f)*config.LOGIC_HEIGHT;
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
static void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, float w, float h, const SDL_Color color, const glm::mat3 &mvpMatrix,float clipWidth, float clipHeight) {
    // corners in local space (top-left origin)
    glm::vec2 corners[4] = {
        {0,0},
        {w,0},
        {w,h},
        {0,h}
    };
    auto config=ApplicationContext::getInstance().get<Config>("config");
    SDL_Vertex verts[4];
    for (int i = 0; i < 4; ++i) {
        SDL_FPoint pos = vertexShader(mvpMatrix, corners[i].x, corners[i].y);
        //裁剪空间->ndc->视口
        verts[i].position.x=(pos.x/clipWidth*0.5f+0.5f)*config.LOGIC_WIDTH;
        verts[i].position.y=(pos.y/clipHeight*0.5f+0.5f)*config.LOGIC_HEIGHT;
        verts[i].color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
    }
    verts[0].tex_coord = SDL_FPoint{ 0.0f, 0.0f };
    verts[1].tex_coord = SDL_FPoint{ 1.0f, 0.0f };
    verts[2].tex_coord = SDL_FPoint{ 1.0f, 1.0f };
    verts[3].tex_coord = SDL_FPoint{ 0.0f, 1.0f };
    int indices[6] = { 0, 1, 2, 2, 3, 0 };
    SDL_RenderGeometry(renderer, texture, verts, 4, indices, 6);
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
            drawCircle(renderer, mvpMatrix,  circleRenderFlagComp.value().radius, color, circleRenderFlagComp.value().segments,cameraComp.viewportWidth,cameraComp.viewportHeight);
        }else if (ecs::getComponent<RectRendererFlag>(scene, entity).has_value()) {
            auto rectRenderFlagComp = ecs::getComponent<RectRendererFlag>(scene, entity);
            float width = rectRenderFlagComp.value().width;
            float height = rectRenderFlagComp.value().height;
            drawTexture(renderer,nullptr,width,height, color,mvpMatrix,cameraComp.viewportWidth,cameraComp.viewportHeight);
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
            drawTexture(renderer, cachedTexture, w, h, color,mvpMatrix,cameraComp.viewportWidth,cameraComp.viewportHeight);
        }else if (ecs::getComponent<ImageRendererFlag>(scene, entity).has_value()) {
            auto imageRendererComp = ecs::getComponent<ImageRendererFlag>(scene, entity);
           auto texture = ResourceManager::getInstance().getTextureCache().get(imageRendererComp.value().textureId);
            if (!texture) {
                SDL_Log("Warning:缓存里没有 %s", imageRendererComp.value().textureId.c_str());
                return;
            }
            // 获取纹理尺寸
            float width = imageRendererComp.value().width;
            float height = imageRendererComp.value().height;
            drawTexture(renderer, texture, width, height, color, mvpMatrix, cameraComp.viewportWidth, cameraComp.viewportHeight);
        }

    }
}