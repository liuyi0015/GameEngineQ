//
// Created by XL0002 on 2026/7/23.
//

#include "ButtonFactory.h"

#include "MouseColliderSystem.h"
#include "../renderer/RenderComponents.h"
#include "../ecs/BaseComponents.h"

Entity ButtonFactory::createTextButton(const std::shared_ptr<Scene> &scene, const SDL_FRect rect, const std::string &fontId, const std::string &surfId, const std::string& texId) {
    Entity entity=scene->createEntity();
    scene->setComponent<MouseColliderFlag>(entity,MouseColliderFlag{rect.w,rect.h});

    scene->setComponent<ecs::Transform>(entity, ecs::Transform{ecs::Position{rect.x,rect.y}, ecs::Rotation{},ecs::Scale{}});
    scene->setComponent<DrawableFlag>(entity,DrawableFlag{1,{255,255,255,255}});
    scene->setComponent<TextRendererFlag>(entity,TextRendererFlag{fontId,surfId,texId});

    return entity;
}

Entity ButtonFactory::createImageButton(const std::shared_ptr<Scene>& scene, const SDL_FRect rect,const std::string& texId) {
    Entity entity=scene->createEntity();
    scene->setComponent<MouseColliderFlag>(entity,MouseColliderFlag{rect.w,rect.h});

    scene->setComponent<ecs::Transform>(entity, ecs::Transform{ecs::Position{rect.x,rect.y}, ecs::Rotation{}, ecs::Scale{}});
    scene->setComponent<DrawableFlag>(entity,DrawableFlag{1,{255,255,255,255}});
    scene->setComponent<ImageRendererFlag>(entity,ImageRendererFlag{texId,rect.w,rect.h});

    return entity;
}
