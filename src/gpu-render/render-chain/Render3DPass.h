//
// Created by XL0002 on 2026/9/29.
//

#ifndef GAMEENGINEQ_RENDER3DPASS_H
#define GAMEENGINEQ_RENDER3DPASS_H


namespace ecs {
    class Scene;
}

class Render3DPass {
public:
    Render3DPass(ecs::Scene* scene){};
    ~Render3DPass()= default;
    void registerPipelines();
};


#endif //GAMEENGINEQ_RENDER3DPASS_H
