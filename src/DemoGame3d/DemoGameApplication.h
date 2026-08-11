//
// Created by XL0002 on 2026/8/5.
//

#ifndef GAMEENGINE_DEMOGAMEAPPLICATION_H
#define GAMEENGINE_DEMOGAMEAPPLICATION_H
#include "../EcsApplication.h"

class DemoGameApplication:public EcsApplication{
public:
    Scene* loadScene(std::string sceneName) override;

    void init() override;
};


#endif //GAMEENGINE_DEMOGAMEAPPLICATION_H
