#ifndef GAMEENGINE_MONOBEHAVIOURSYSTEM_H
#define GAMEENGINE_MONOBEHAVIOURSYSTEM_H
#include "BaseComponents.h"
#include "Scene.h"
#include "System.h"

namespace ecs {

    template<typename TFlagComponent>
    class MonoBehaviourSystem : public System{
    private:
        [[nodiscard]] bool checkEnabled(Entity entity) const {
            auto flag=scene->getComponent<TFlagComponent>(entity);
            if (!flag.has_value()) {
                return false;
            }
            auto enabled=scene->getComponent<Enabled>(entity);
            if (!enabled.has_value()) {
                return false;
            }
            return enabled.value().value;
        }
    protected:
        Entity curEntity;
    public:
        explicit MonoBehaviourSystem(std::shared_ptr<Scene> scene)
            : System(std::move(scene)), curEntity(-1) {
        }
        void start() override {
            for (const auto entity:scene->getEntities()) {
                if (!checkEnabled(entity)) {
                    continue;
                }
                curEntity=entity;
                onStart();
            }
        };
        virtual void onStart() {};
        void update(double deltaTime) override {
            for (const auto entity:scene->getEntities()) {
                if (!checkEnabled(entity)) {
                    continue;
                }
                curEntity=entity;
                onUpdate(deltaTime);
            }
        };
        virtual void onUpdate(float deltaTime) {};
        void fixed_update(double deltaTime) override {
            for (const auto entity:scene->getEntities()) {
                if (!checkEnabled(entity)) {
                    continue;
                }
                curEntity=entity;
                onFixedUpdate(deltaTime);
            }
        };
        virtual void onFixedUpdate(float deltaTime) {};
        void draw() override {
            for (const auto entity:scene->getEntities()) {
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
