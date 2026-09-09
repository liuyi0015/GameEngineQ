//
// Created by XL0002 on 2026/9/4.
//

#include "UploadSystem.h"

#include "Pipelines.h"
#include "Render2DComponents.h"
#include "../../core/ecs/Util.h"
#include "../../core/transform/transform2d/Transform2dComponents.h"
#include "../../core/transform/transform2d/TransformUtil.h"


void UploadSystem::draw() {
    gpu->vert_buffer.clear();
    gpu->index_buffer.clear();
    for (const auto entity:ecs::getEntities<Drawable2DFlag>(scene)) {
        auto drawableFlag= ecs::getComponent<Drawable2DFlag>(scene, entity).value();
        //上传顶点
        renderContext->vertStarts[entity]=gpu->vert_buffer.size();
        renderContext->vertCounts[entity]=drawableFlag.mesh.vertices.size();
        for (int i=0;i<drawableFlag.mesh.vertices.size();i++) {
            glm::vec4 fcolor={
                drawableFlag.material.color.r/255.0f,
                drawableFlag.material.color.g/255.0f,
                drawableFlag.material.color.b/255.0f,
                drawableFlag.material.color.a/255.0f,
            };;
            VertexAttrib v {drawableFlag.mesh.vertices[i].pos,drawableFlag.mesh.vertices[i].uv,fcolor};
            gpu->vert_buffer.push_back(v);
        }
        //上传索引
        renderContext->indexStarts[entity]=gpu->index_buffer.size();
        renderContext->indexCounts[entity]=drawableFlag.mesh.indices.size();
        for (int i=0;i<drawableFlag.mesh.indices.size();i++) {
            glm::ivec3 index=drawableFlag.mesh.indices[i];
            gpu->index_buffer.push_back(index);
        }
    }
    //合成器顶点：屏幕矩形
    renderContext->composeVertStart= gpu->vert_buffer.size();
    VertexAttrib vert0={};
    vert0.pos={-1,1};
    vert0.uv={0,0};
    gpu->vert_buffer.push_back(vert0);
    VertexAttrib vert1={};
    vert1.pos={1,1};
    vert1.uv={1,0};
    gpu->vert_buffer.push_back(vert1);
    VertexAttrib vert2={};
    vert2.pos={1,-1};
    vert2.uv={1,1};
    gpu->vert_buffer.push_back(vert2);
    VertexAttrib vert3={};
    vert3.pos={-1,-1};
    vert3.uv={0,1};
    gpu->vert_buffer.push_back(vert3);
    renderContext->composeIndexStart= gpu->index_buffer.size();
    auto index0=glm::ivec3(0,1,2);
    auto index1=glm::ivec3(0,2,3);
    gpu->index_buffer.push_back(index0);
    gpu->index_buffer.push_back(index1);
}
