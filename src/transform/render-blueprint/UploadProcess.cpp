//
// Created by XL0002 on 2026/9/4.
//

#include "UploadProcess.h"

#include "Pipelines.h"
#include "Render2DComponents.h"
#include "../../core/ecs/Util.h"
#include "../transform2d/TransformUtil.h"
#include "../../core/ResourceManager.hpp"


//渲染蓝图里唯一依赖transform的函数，但是又不好提出去，怪怪的
static Uniform * collectUniform(ecs::Scene* scene,Entity entity,Entity camera) {
    auto drawableFlag= ecs::getComponent<Drawable2DFlag>(scene, entity);
    auto transformComp = ecs::getComponent<Transform2DComp>(scene, entity);
    auto cameraComp = ecs::getComponent<Camera2DComp>(scene, camera);
    auto cameraTransformComp=ecs::getComponent<Transform2DComp>(scene, camera);
    assert(transformComp.has_value()&&drawableFlag.has_value()&&cameraComp.has_value()&&cameraTransformComp.has_value());
    auto cameraWorldTransform=TransformUtil::computeLocalToWorldTransform(cameraTransformComp.value(),scene);
    auto worldTransform = TransformUtil::computeLocalToWorldTransform(transformComp.value(),scene);
    auto modelMatrix=TransformUtil::transformToMatrix(worldTransform);
    auto viewMatrix = TransformUtil::getReverseTransformToMatrix(cameraWorldTransform);
    int clipW=cameraComp->captureWidth;
    int clipH=cameraComp->captureHeight;
    auto projectMatrix=glm::mat3(
        2.0f/clipW,0,0,
        0,2.0f/clipH,0,
        0,0,1
        );//没有投影但要坐标归一化
    auto mvpMatrix=projectMatrix*viewMatrix*modelMatrix;
    auto texture= ResourceManager::getInstance().getSurfaceCache().get(drawableFlag.value().material.texResourceId);
    auto* uniform=new Uniform2D();
    uniform->mvpMatrix=mvpMatrix;
    if (texture!=nullptr) {
        uniform->texture=new ColorBuffer(texture);
    }
    // uniform.color=drawableFlag.value().color;
    return uniform;
}
void UploadProcess::draw() {
    //上传uniform
    Entity camera=ecs::searchEntity<Camera2DComp>(scene)[0];
    gpu->vert_buffers[0]->vertices.clear();
    gpu->index_buffers[0]->indices.clear();
    for (const auto entity:ecs::getEntities<Drawable2DFlag>(scene)) {
        auto drawableFlag= ecs::getComponent<Drawable2DFlag>(scene, entity).value();
        //上传顶点
        renderContext->vertOffsets[entity]=gpu->vert_buffers[0]->vertices.size();
        renderContext->vertCounts[entity]=drawableFlag.mesh.vertices.size();
        for (const auto & vertice : drawableFlag.mesh.vertices) {
            gpu->vert_buffers[0]->vertices.push_back(vertice);
        }
        //上传索引
        renderContext->indexOffsets[entity]=gpu->index_buffers[0]->indices.size();
        renderContext->indexCounts[entity]=drawableFlag.mesh.indices.size();
        for (const auto & index : drawableFlag.mesh.indices) {
            gpu->index_buffers[0]->indices.push_back(index);
        }
        Uniform* uniform=collectUniform(scene,entity,camera);
        gpu->uniforms[0]->uniforms.push_back(uniform);
        renderContext->uniformOffsets[entity]=gpu->uniforms[0]->uniforms.size()-1;
    }
    //合成器顶点：屏幕矩形
    gpu->vert_buffers[1]->vertices.clear();
    renderContext->composeVertStart= gpu->vert_buffers[1]->vertices.size();
    VertexAttrib2D vert0={};
    //直接填ndc坐标，也是先x后y
    vert0.pos={-1,1};//左上
    vert0.uv={0,0};
    gpu->vert_buffers[1]->vertices.push_back(vert0);
    VertexAttrib2D vert1={};
    vert1.pos={1,1};//右上
    vert1.uv={1,0};
    gpu->vert_buffers[1]->vertices.push_back(vert1);
    VertexAttrib2D vert2={};
    vert2.pos={1,-1};//右下
    vert2.uv={1,1};
    gpu->vert_buffers[1]->vertices.push_back(vert2);
    VertexAttrib2D vert3={};
    vert3.pos={-1,-1};//左下
    vert3.uv={0,1};
    gpu->vert_buffers[1]->vertices.push_back(vert3);
    renderContext->composeIndexStart= gpu->index_buffers[0]->indices.size();
    auto index0=glm::ivec3(0,1,2);
    auto index1=glm::ivec3(0,2,3);
    gpu->index_buffers[0]->indices.push_back(index0);
    gpu->index_buffers[0]->indices.push_back(index1);

}
