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
    SDL_Texture* target;
public:
    explicit UIRenderSystem(ecs::Scene* scene,SDL_Texture* target)
        : System(scene){
        renderer=ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
        this->target=target;
    }
    ~UIRenderSystem() override;
    void start() override;
    void update(double deltaTime) override{};
    void fixed_update(double deltaTime) override{};
    void draw() override;

};


#endif //GAMEENGINE_UIRENDERSYSTEM_H
