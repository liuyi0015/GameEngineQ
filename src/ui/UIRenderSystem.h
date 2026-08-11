//
// Created by XL0002 on 2026/8/11.
//

#ifndef GAMEENGINE_UIRENDERSYSTEM_H
#define GAMEENGINE_UIRENDERSYSTEM_H
#include "UIComponents.h"
#include "../Context.hpp"
#include "../ecs/System.h"
#include "SDL3/SDL_render.h"
#include "../Config.h"
#include "../ecs/Util.h"

class UIRenderSystem :public ecs::System{
private:
    SDL_Renderer* renderer;
    SDL_Texture* canvas;
public:
    explicit UIRenderSystem(Scene* scene,SDL_Renderer* renderer)
        : System(scene),renderer(renderer){
        Entity canvas_item=ecs::getEntities<CanvasComp>(scene)[0];
        auto canvas_prop = ecs::getComponent<CanvasComp>(scene,canvas_item).value();
        canvas=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,
            canvas_prop.width,canvas_prop.height);
    }
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;

};


#endif //GAMEENGINE_UIRENDERSYSTEM_H
