//
// Created by XL0002 on 2026/8/13.
//

#include "ButtonCallbackSystem.h"

#include "../../2d/transform2d/TransformUtil.h"
#include "../../2d/renderer2d/RenderComponents.h"
#include "../../EventDispatcher.h"

static std::vector<size_t> subscribeIds;
ButtonCallbackSystem::~ButtonCallbackSystem() {
    for (auto id:subscribeIds) {
        EventDispatcher::getInstance().unsubscribe(id);
    }

}
void ButtonCallbackSystem::onStart() {
    size_t id= EventDispatcher::getInstance().subscribe("say hello",[](const std::any& param) {
        std::cout<<"hello,GameEngineQ!"<<std::endl;
        EventDispatcher::getInstance().publish("quit",nullptr);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("text1-tex hover",[this](const std::any& param) {
        auto drawableFlag=ecs::getComponent<DrawableFlag>(scene,curEntity).value();
        auto hoverFlag=ecs::getComponent<HoverColorFlag>(scene,curEntity).value();
        drawableFlag.color=hoverFlag.hoverColor;
        ecs::setComponent<DrawableFlag>(scene,curEntity,drawableFlag);
    });
    subscribeIds.push_back(id);
    id=EventDispatcher::getInstance().subscribe("text1-tex unhover",[this](const std::any& param) {
        auto drawableFlag=ecs::getComponent<DrawableFlag>(scene,curEntity).value();
        auto hoverFlag=ecs::getComponent<HoverColorFlag>(scene,curEntity).value();
        drawableFlag.color=hoverFlag.unhoverColor;
        ecs::setComponent<DrawableFlag>(scene,curEntity,drawableFlag);
    });
    subscribeIds.push_back(id);
}
