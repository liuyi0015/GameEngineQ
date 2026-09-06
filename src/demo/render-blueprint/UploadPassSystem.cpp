//
// Created by XL0002 on 2026/9/4.
//

#include "UploadPassSystem.h"

#include "Pipelines.h"
#include "Render2DComponents.h"
#include "../../core/ecs/Util.h"
#include "../../core/transform/transform2d/Transform2dComponents.h"
#include "../../core/transform/transform2d/TransformUtil.h"


void UploadPassSystem::draw() {
    gpu->vert_buffer.clear();
    gpu->index_buffer.clear();
    for (const auto entity:ecs::getEntities<Drawable2DFlag>(scene)) {
        auto drawableFlag= ecs::getComponent<Drawable2DFlag>(scene, entity);

        //上传顶点
        renderContext->vertStarts[entity]=gpu->vert_buffer.size();
        renderContext->vertCounts[entity]=drawableFlag.value().shape.points.size();
        for (int i=0;i<drawableFlag.value().shape.points.size();i++) {
            glm::vec4 fcolor={
                drawableFlag.value().material.color.r/255.0f,
                drawableFlag.value().material.color.g/255.0f,
                drawableFlag.value().material.color.b/255.0f,
                drawableFlag.value().material.color.a/255.0f,
            };;
            auto v=new VertexAttrib {drawableFlag.value().shape.points[i],drawableFlag.value().material.uvs[i],fcolor,{0,0}};
            gpu->vert_buffer.push_back(v);
        }
        //上传索引
        renderContext->indexStarts[entity]=gpu->index_buffer.size();
        renderContext->indexCounts[entity]=drawableFlag.value().shape.indices.size();
        for (int i=0;i<drawableFlag.value().shape.indices.size();i++) {
            glm::ivec3 index=drawableFlag.value().shape.indices[i];
            gpu->index_buffer.push_back(index);
        }
    }
    //合成器顶点：屏幕矩形
    renderContext->composeVertStart= gpu->vert_buffer.size();
    auto vert0=new VertexAttrib();
    vert0->pos={-1,1};
    vert0->uv={0,0};
    gpu->vert_buffer.push_back(vert0);
    auto vert1=new VertexAttrib();
    vert0->pos={1,1};
    vert0->uv={1,0};
    gpu->vert_buffer.push_back(vert1);
    auto vert2=new VertexAttrib();
    vert0->pos={1,-1};
    vert0->uv={1,1};
    gpu->vert_buffer.push_back(vert2);
    auto vert3=new VertexAttrib();
    vert0->pos={-1,-1};
    vert0->uv={0,1};
    gpu->vert_buffer.push_back(vert3);
    renderContext->composeIndexStart= gpu->index_buffer.size();
    auto index0=glm::ivec3(0,1,2);
    auto index1=glm::ivec3(0,2,3);
    gpu->index_buffer.push_back(index0);
    gpu->index_buffer.push_back(index1);
}
