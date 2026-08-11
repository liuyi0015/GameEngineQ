//
// Created by XL0002 on 2026/8/5.
//

#include "Prefabs.h"

#include "../3d/render3d-soft/RenderComponents.h"
#include "../ecs/Scene.h"
#include "../ecs/Util.h"

Entity Prefabs::cube(Scene* scene) {
    Entity entity=ecs::createEntity(scene);
    Position3D ps[8]={
        {0,0,0},
        {0,0,1},
        {0,1,0},
        {0,1,1},
        {1,0,0},
        {1,0,1},
        {1,1,0},
        {1,1,1}
    };
    std::vector<Vertex3D> vertices;
    for (auto & p : ps) {
        vertices.push_back(Vertex3D{p,SDL_Color{255,255,255},{0,0}});
    }
    ecs::setComponent<Drawable3DFlag>(scene,entity, {SDL_Color{255,255,255,255},std::move(vertices)});
    ecs::setComponent<Transform3D>(scene,entity, {});
    return entity;
}
