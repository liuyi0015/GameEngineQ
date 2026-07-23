//
// Created by XL0002 on 2026/7/16.
//

#ifndef GAMEENGINE_MOVESYSTEM_H
#define GAMEENGINE_MOVESYSTEM_H
#include <memory>
#include "../ecs/MonoBehaviourSystem.h"
#include "../ecs/Scene.h"

struct MoveFlag {
    float x;
    float y;
    float width;
    float height;
    float speed;
    int currentEdge = 0;
};//标记组件

class MoveSystem : public ecs::MonoBehaviourSystem<MoveFlag> {
public:
    explicit MoveSystem(std::shared_ptr<Scene> scene)
        : ecs::MonoBehaviourSystem<MoveFlag>(std::move(scene)) {
        
    }

    void onStart() override{};
    void onUpdate(float deltaTime) override{};
    void onFixedUpdate(float deltaTime) override;
    void onDraw() const override{};
};



#endif //GAMEENGINE_MOVESYSTEM_H
