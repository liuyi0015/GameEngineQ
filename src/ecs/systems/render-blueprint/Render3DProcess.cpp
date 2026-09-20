//
// Created by XL0002 on 2026/9/16.
//

#include "Render3DProcess.h"

#include "Pipelines.h"
#include "RenderComponents.h"
#include "../../../ecs/Util.h"
#include "../../../transform/transform3d/Transform3dComponents.h"
#include "../../../transform/transform3d/Transform3DUtil.h"
#include "../../../core/ResourceManager.hpp"
#include "../../util/TransformSceneUtil.h"

void Render3DProcess::initBuffers() {
    gpu->vert_buffers.push_back(new VertexBuffer());
    vert_buffer_index=gpu->vert_buffers.size()-1;
    gpu->index_buffers.push_back(new IndiceBuffer());
    index_buffer_index=gpu->index_buffers.size()-1;
    gpu->uniform_buffers.push_back(new UniformBuffer());
    uniform_buffer_index=gpu->uniform_buffers.size()-1;
}

void Render3DProcess::registerPipelines() {

    //就用默认的
    auto pipeline=new Default3DPipeline();
    gpu->pipelines["3d_pipeline"]=pipeline;
}

//渲染蓝图里唯一依赖transform的函数，但是又不好提出去，怪怪的
static Uniform * collectUniform(ecs::Scene* scene,Entity entity,Entity camera) {
    auto drawableFlag= ecs::getComponent<Drawable3DFlag>(scene, entity);
    auto transformComp = ecs::getComponent<Transform3DComp>(scene, entity);
    auto cameraComp = ecs::getComponent<Camera3DComp>(scene, camera);
    auto cameraTransformComp=ecs::getComponent<Transform3DComp>(scene, camera);
    assert(transformComp.has_value()&&drawableFlag.has_value()&&cameraComp.has_value()&&cameraTransformComp.has_value());
    // auto cameraWorldTransform=TransformSceneUtil::computeLocalToWorldTransform(cameraTransformComp.value(),scene);
    // auto worldTransform = TransformSceneUtil::computeLocalToWorldTransform(transformComp.value(),scene);
    // auto modelMatrix=Transform3DUtil::transformToMatrix(worldTransform);
    // auto viewMatrix = Transform3DUtil::getReverseTransformToMatrix(cameraWorldTransform);
    int clipW=cameraComp->captureWidth;
    int clipH=cameraComp->captureHeight;
    auto projectMatrix=glm::mat4(
        );
    // auto mvpMatrix=projectMatrix*viewMatrix*modelMatrix;
    auto texture= ResourceManager::getInstance().getSurfaceCache().get(drawableFlag.value().material.texResourceId);
    auto* uniform=new Uniform3D();
    // uniform->mvpMatrix=mvpMatrix;
    if (texture!=nullptr) {
        uniform->texture=new ColorBuffer(texture);
    }
    // uniform.color=drawableFlag.value().color;
    return uniform;
}
void Render3DProcess::uploadData() {
    Entity camera=ecs::searchEntity<Camera3DComp>(scene)[0];

    for (const auto entity:ecs::getEntities<Drawable3DFlag>(scene)) {
        auto drawableFlag= ecs::getComponent<Drawable3DFlag>(scene, entity).value();
        //上传顶点
        renderContext->vertOffsets[entity]=gpu->vert_buffers[this->vert_buffer_index]->vertices.size();
        renderContext->vertCounts[entity]=drawableFlag.mesh.vertices.size();
        for (const auto & vertice : drawableFlag.mesh.vertices) {
            gpu->vert_buffers[this->vert_buffer_index]->vertices.push_back(vertice);
        }
        //上传索引
        renderContext->indexOffsets[entity]=gpu->index_buffers[this->index_buffer_index]->indices.size();
        renderContext->indexCounts[entity]=drawableFlag.mesh.indices.size();
        for (const auto & index : drawableFlag.mesh.indices) {
            gpu->index_buffers[this->index_buffer_index]->indices.push_back(index);
        }
        Uniform* uniform=collectUniform(scene,entity,camera);
        gpu->uniform_buffers[this->uniform_buffer_index]->uniforms.push_back(uniform);
        renderContext->uniformOffsets[entity]=gpu->uniform_buffers[this->uniform_buffer_index]->uniforms.size()-1;
    }
}

void Render3DProcess::draw() {
    //目前只拿第一个摄像机
    Entity camera=ecs::searchEntity<Camera3DComp>(scene)[0];
    ColorBuffer* target=ecs::getComponent<Camera3DComp>(scene,camera).value().target;
    //按pipeline分组排序
    //或者说先整体排序再分组
    std::vector<std::pair<int,Entity>> drawableEntities;
    for (const auto entity:ecs::getEntities<Drawable3DFlag>(scene)) {
        if (ecs::getComponent<Drawable3DFlag>(scene, entity).has_value()) {
            auto zOrder = ecs::getComponent<Drawable3DFlag>(scene, entity).value().z_order;
            drawableEntities.push_back({zOrder,entity});
        }
    }
    // sort by zOrder (ascending), stable to preserve insertion order for equal z
    std::stable_sort(drawableEntities.begin(), drawableEntities.end(),
                     [](const auto &a, const auto &b){ return a.first < b.first; });
    //绑定渲染目标
    RenderPass render_pass(target,this->vert_buffer_index,this->index_buffer_index,this->uniform_buffer_index);
    //绑定渲染管线 todo 未分组
    render_pass.cur_pipeline=gpu->pipelines["3d_pipeline"];

    target->clear({0,0,0,1});//黑屏时调成红色用来debug
    //顶点分组
    for (int i=0;i<drawableEntities.size();i++) {
        Entity entity=drawableEntities[i].second;
        auto indexOffset=renderContext->indexOffsets[entity];
        auto indexCount=renderContext->indexCounts[entity];
        auto vertOffset=renderContext->vertOffsets[entity];
        auto uniformOffset=renderContext->uniformOffsets[entity];
        gpu->drawcall(render_pass,indexOffset,indexCount,vertOffset,uniformOffset);
    }
}

void Render3DProcess::endFrame() {

    //清理
    gpu->vert_buffers[this->vert_buffer_index]->vertices.clear();
    gpu->index_buffers[this->index_buffer_index]->indices.clear();
    for (auto* uniform:gpu->uniform_buffers[this->uniform_buffer_index]->uniforms) {
        delete uniform;
    }
    gpu->uniform_buffers[this->uniform_buffer_index]->uniforms.clear();
}

