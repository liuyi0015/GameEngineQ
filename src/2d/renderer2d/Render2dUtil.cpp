//
// Created by XL0002 on 2026/8/17.
//

#include "Render2dUtil.h"
#include "../../Context.hpp"
#include "../../Config.h"
//把顶点从模型空间换到裁剪空间
static SDL_FPoint vertexShader(const glm::mat3 &mvpMatrix, float lx, float ly) {
    glm::vec3 p = mvpMatrix * glm::vec3(lx, ly, 1.0f);
    return SDL_FPoint{ p.x, p.y };
}
void Render2dUtil::drawCircle(SDL_Renderer *renderer, const glm::mat3 mvpMatrix, float radius, const SDL_Color &color,
    int segments, float clipWidth, float clipHeight, float viewportW, float viewportH) {
        if (segments < 3) segments = 3;
        const int numVerts = segments + 1;
        std::vector<SDL_Vertex> verts;
        verts.reserve(numVerts);
        // center at local (0,0)
        // 圆心顶点（索引 0）
        SDL_Vertex centerV{};
        SDL_FPoint pos = vertexShader(mvpMatrix, 0.0f, 0.0f);
        centerV.position.x = (pos.x / clipWidth * 0.5f + 0.5f) * viewportW;
        centerV.position.y = (pos.y / clipHeight * 0.5f + 0.5f) * viewportH;
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
            v.position.x=(pos.x/clipWidth*0.5f+0.5f)*viewportW;
            v.position.y=(pos.y/clipHeight*0.5f+0.5f)*viewportH;
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
//画texture的uv多边形内部到世界里，然后用相机取cw/ch大小，显示在vw/vh的画布上
void Render2dUtil::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, glm::vec2 uvs[], const SDL_Color color,
                               const glm::mat3 &mvpMatrix, float cameraWidth, float cameraHeight, float viewportW, float viewportH) {

    SDL_Vertex verts[4];
    for (int i = 0; i < 4; ++i) {
        SDL_FPoint pos = vertexShader(mvpMatrix, uvs[i].x, uvs[i].y);
        //裁剪空间->ndc->视口
        verts[i].position.x=(pos.x/cameraWidth*0.5f+0.5f)*viewportW;
        verts[i].position.y=(pos.y/cameraHeight*0.5f+0.5f)*viewportH;
        verts[i].color = SDL_FColor{ color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f };
    }
    verts[0].tex_coord = SDL_FPoint{ 0.0f, 0.0f };
    verts[1].tex_coord = SDL_FPoint{ 1.0f, 0.0f };
    verts[2].tex_coord = SDL_FPoint{ 1.0f, 1.0f };
    verts[3].tex_coord = SDL_FPoint{ 0.0f, 1.0f };
    int indices[6] = { 0, 1, 2, 2, 3, 0 };
    SDL_RenderGeometry(renderer, texture, verts, 4, indices, 6);
}
