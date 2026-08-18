//
// Created by XL0002 on 2026/8/13.
//

#ifndef GAMEENGINE_BUTTONRECTREDSYSTEM_H
#define GAMEENGINE_BUTTONRECTREDSYSTEM_H
#include "../GameComponents.h"
#include "../../ecs/MonoBehaviourSystem.h"
#include "../../ui/UIComponents.h"


class ButtonCallbackSystem:public ecs::MonoBehaviourSystem<HoverColorFlag>{
protected:
    public:
    explicit ButtonCallbackSystem(ecs::Scene* scene)
        : ecs::MonoBehaviourSystem<HoverColorFlag>(scene) {

    }
    ~ButtonCallbackSystem()override;

    void onStart() override;
    void onUpdate(double deltaTime) override{};
    void onFixedUpdate(double deltaTime) override{};
    void onDraw() const override{};
};


#endif //GAMEENGINE_BUTTONRECTREDSYSTEM_H
