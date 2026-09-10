//
// Created by XL0002 on 2026/7/22.
//

#include "Render2DSystem.h"

#include <algorithm>
#include <cassert>

#include "Pipelines.h"
#include "Render2DComponents.h"
#include "../../core/ResourceManager.hpp"
#include "../../core/ecs/Util.h"
#include "../../soft-render/SoftRenderer2D.h"
#include "../../core/transform/transform2d/Transform2dComponents.h"
#include "../../core/transform/transform2d/TransformUtil.h"

void Render2DSystem::start() {
    //就用默认的
    auto pipeline=new DefaultPipeline();
    gpu->pipelines["default_pipeline"]=pipeline;
    //让其他类也可以通过名字访问target
    gpu->render_targets[this->targetName]=target;
}

static glm::mat3 getViewMatrix(const Transform &cameraTransform) {
    Transform transform=cameraTransform;
    transform.scale={1.0f,1.0f};//摄像机的scale不影响视图变换
    glm::mat3 viewMatrix=TransformUtil::getReverseTransformToMatrix(transform);
    return viewMatrix;
}

static Uniform * collectUniform(ecs::Scene* scene,Entity entity,Entity camera) {
    auto drawableFlag= ecs::getComponent<Drawable2DFlag>(scene, entity);
    auto transformComp = ecs::getComponent<TransformComp>(scene, entity);
    auto cameraComp = ecs::getComponent<CameraComp>(scene, camera);
    auto cameraTransformComp=ecs::getComponent<TransformComp>(scene, camera);
    assert(transformComp.has_value()&&drawableFlag.has_value()&&cameraComp.has_value()&&cameraTransformComp.has_value());
    auto cameraWorldTransform=TransformUtil::computeLocalToWorldTransform(cameraTransformComp.value(),scene);
    auto worldTransform = TransformUtil::computeLocalToWorldTransform(transformComp.value(),scene);
    auto modelMatrix=TransformUtil::transformToMatrix(worldTransform);
    auto viewMatrix = getViewMatrix(cameraWorldTransform);
    int clipW=cameraComp->captureWidth;
    int clipH=cameraComp->captureHeight;
    auto projectMatrix=glm::mat3(
        2.0f/clipW,0,0,
        0,2.0f/clipH,0,
        0,0,1
        );//没有投影但要坐标归一化
    auto mvpMatrix=projectMatrix*viewMatrix*modelMatrix;
    auto texture= ResourceManager::getInstance().getSurfaceCache().get(drawableFlag.value().material.texResourceId);
    auto* uniform=new Uniform();
    uniform->mvpMatrix=mvpMatrix;
    if (texture!=nullptr) {
        uniform->texture=new ColorBuffer(texture);
    }
    // uniform.color=drawableFlag.value().color;
    return uniform;
}
void Render2DSystem::draw() {
    //按pipeline分组排序，提交命令 todo 未分组
    //或者说先整体排序再分组
    std::vector<std::pair<int,Entity>> drawableEntities;
    for (const auto entity:ecs::getEntities<Drawable2DFlag>(scene)) {
        if (ecs::getComponent<Drawable2DFlag>(scene, entity).has_value()) {
            auto zOrder = ecs::getComponent<Drawable2DFlag>(scene, entity).value().z_order;
            drawableEntities.push_back({zOrder,entity});
        }
    }
    // sort by zOrder (ascending), stable to preserve insertion order for equal z
    std::stable_sort(drawableEntities.begin(), drawableEntities.end(),
                     [](const auto &a, const auto &b){ return a.first < b.first; });

    const RenderPass render_pass{"default_pipeline",target};
    gpu->cur_renderpass=render_pass;
    target->clear({0,0,0,1});//黑屏时调成红色用来debug
    //顶点分组
    for (int i=0;i<drawableEntities.size();i++) {
        Entity entity=drawableEntities[i].second;
        //上传uniform
        Uniform* uniform=collectUniform(scene,entity,camera);
        auto indexStart=renderContext->indexStarts[entity];
        auto indexCount=renderContext->indexCounts[entity];
        auto vertStart=renderContext->vertStarts[entity];
        gpu->drawcall(indexStart,indexCount,vertStart,uniform);
        delete uniform;
    }
}
