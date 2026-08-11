#ifndef GAMEENGINE_MONOBEHAVIOURSYSTEM_H
#define GAMEENGINE_MONOBEHAVIOURSYSTEM_H
#include "BaseComponents.h"
#include "Scene.h"
#include "System.h"
#include "Util.h"

namespace ecs {
//乱序处理组件，或者说，按放进场景的顺序
    template<typename TFlagComponent>
    class MonoBehaviourSystem : public System{
    private:
        [[nodiscard]] bool checkEnabled(Entity entity) const {
            auto enabled=getComponent<Enabled>(scene, entity);
            if (!enabled.has_value()) {
                return false;
            }
            return enabled.value().value;
        }
    protected:
        Entity curEntity;
    public:
        explicit MonoBehaviourSystem(Scene* scene)
            : System(scene), curEntity(-1) {
        }
        void start() override {
            for (const auto entity:getEntities<TFlagComponent>(scene)) {
                if (!checkEnabled(entity)) {
                    continue;
                }
                curEntity=entity;
                onStart();
            }
        };
        virtual void onStart() {};
        void update(double deltaTime) override {
            for (const auto entity:getEntities<TFlagComponent>(scene)) {
                if (!checkEnabled(entity)) {
                    continue;
                }
                curEntity=entity;
                onUpdate(deltaTime);
            }
        };
        virtual void onUpdate(double deltaTime) {};
        void fixed_update(double deltaTime) override {
            for (const auto entity:getEntities<TFlagComponent>(scene)) {
                if (!checkEnabled(entity)) {
                    continue;
                }
                curEntity=entity;
                onFixedUpdate(deltaTime);
            }
        };
        virtual void onFixedUpdate(double deltaTime) {};
        void draw() override {
            for (const auto entity:getEntities<TFlagComponent>(scene)) {
                if (!checkEnabled(entity)) {
                    continue;
                }
                curEntity=entity;
                onDraw();
            }
        };
        virtual void onDraw() const{};
    };
}


#endif //GAMEENGINE_MONOBEHAVIOURSYSTEM_H
