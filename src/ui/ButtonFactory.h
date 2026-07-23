//
// Created by XL0002 on 2026/7/23.
//

#ifndef GAMEENGINE_BUTTON_H
#define GAMEENGINE_BUTTON_H

#include "../ecs/Scene.h"
#include "SDL3/SDL_rect.h"

class ButtonFactory {
    public:
    static Entity createTextButton(const std::shared_ptr<Scene> &scene,SDL_FRect rect,const std::string& fontId,const std::string& surfId,const std::string& texId,std::string event);
    static Entity createImageButton(const std::shared_ptr<Scene>& scene,SDL_FRect rect,const std::string& texId);
};


#endif //GAMEENGINE_BUTTON_H
