//
// Created by XL0002 on 2026/9/29.
//

#include "Render3DPass.h"

#include <iostream>
#include <vector>

#include "SDL3/SDL_gpu.h"


static SDL_GPUGraphicsPipeline *define3DPipeline(SDL_GPUDevice *device, SDL_Window *window) {
    // 渲染管线创建（启动时）
    // （1）输出目标格式定义，这里就是接受颜色
    SDL_GPUColorTargetDescription colorTargetDesc = {
        .format = SDL_GetGPUSwapchainTextureFormat(device, window),
        .blend_state = SDL_GPU_BLENDFACTOR_INVALID// 后续可以在这里启用混合
    };
    // ----目标信息创建
    SDL_GPUGraphicsPipelineTargetInfo targetInfo = {
        .color_target_descriptions = &colorTargetDesc,
        .num_color_targets = 1
    };
    // 渲染管线创建（2）着色器
    // ----顶点着色器
    size_t vsCodeSize;
    void* vsCode = SDL_LoadFile("shaders/3d_color.vert.spv", &vsCodeSize);
    SDL_GPUShaderCreateInfo vsCreateInfo = {
        .code_size = vsCodeSize,
        .code = (Uint8*)vsCode,
        .entrypoint = "main",
        .format = SDL_GetGPUShaderFormats(device),
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,//这里真正告诉GPU这是顶点着色器
        .num_uniform_buffers = 1
    };
    SDL_GPUShader* vertexShader = SDL_CreateGPUShader(device, &vsCreateInfo);
    if (vertexShader == nullptr) {
        std::cerr << "顶点着色器创建失败!" << std::endl;
        return nullptr;
    }
    SDL_free(vsCode);
    // ----片元着色器
    size_t fsCodeSize;
    void* fsCode = SDL_LoadFile("shaders/color.frag.spv", &fsCodeSize);
    SDL_GPUShaderCreateInfo fsCreateInfo = {
        .code_size = fsCodeSize,
        .code = (Uint8*)fsCode,
        .entrypoint = "main",
        .format = SDL_GetGPUShaderFormats(device),
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
    };
    SDL_GPUShader* fragmentShader = SDL_CreateGPUShader(device, &fsCreateInfo);
    if (fragmentShader == nullptr) {
        std::cerr << "片元着色器创建失败！" << std::endl;
        return nullptr;
    }
    SDL_free(fsCode);

    // （3）输入信息格式定义
    // ----顶点属性信息
    // --------0号位: 位置XY
    SDL_GPUVertexAttribute vertexAttributes[]{
        //pos
        {
            .location = 0,
            .buffer_slot=0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = 0,
        },
        //color(4)
        {
            .location = 1,
            .buffer_slot=0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
            .offset =  sizeof(float)*3
        }
    };

    // ----顶点缓冲区描述
    SDL_GPUVertexBufferDescription vertexBufferDesc = {
        .slot =0,//定义缓冲区编号
        .pitch =  sizeof(float) * 7,
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX
    };
    // ----总格式
    SDL_GPUVertexInputState vertexInputState = {
        .vertex_buffer_descriptions = &vertexBufferDesc,
        .num_vertex_buffers = 1,
        .vertex_attributes = vertexAttributes,
        .num_vertex_attributes = 2,
    };
    // 渲染管线创建（4）最终创建
    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .vertex_input_state = vertexInputState,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = targetInfo
    };
    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineCreateInfo);
    if (pipeline == nullptr) {
        std::cerr << "渲染管线创建失败！" << std::endl;
        return nullptr;
    }
    SDL_ReleaseGPUShader(device, vertexShader);
    SDL_ReleaseGPUShader(device, fragmentShader);
    return pipeline;
}

//上传过程并不关心数据含义
static void uploadToBuffer(SDL_GPUDevice *device,SDL_GPUBuffer* buffer,size_t dstOffset,void* data, size_t dataBytes) {

    // 顶点缓冲区数据上传（1）创建传输缓冲区，内存区
    SDL_GPUTransferBufferCreateInfo tboCreateInfo = {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = (Uint32)dataBytes
    };
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &tboCreateInfo);

    // 顶点缓冲区数据上传（2）顶点数据复制到传输缓冲区
    void* transferData = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    memcpy(transferData, data, dataBytes);
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);

    // 顶点缓冲区数据上传（3）创建“上传用”命令缓冲区，开始复制过程
    SDL_GPUCommandBuffer* uploadCommandBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCommandBuffer);

    // 顶点缓冲区数据上传（4）将整块传输缓冲区的内容上传到顶点缓冲区
    SDL_GPUTransferBufferLocation location = {
        .transfer_buffer = transferBuffer,
        .offset = 0
    };
    SDL_GPUBufferRegion region = {
        .buffer = buffer,
        .offset = (Uint32)dstOffset,
        .size = (Uint32)dataBytes
    };
    SDL_UploadToGPUBuffer(copyPass, &location, &region, false);

    // 顶点缓冲区数据上传（5）结束复制过程，提交“上传用”命令缓冲区，释放传输缓冲区
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCommandBuffer);
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
}

static SDL_GPUBuffer *uploadColor3DVertexToNewBuffer(SDL_GPUDevice *device, std::vector<ColorVertex3D> vertices) {

    //创建顶点缓冲区
    SDL_GPUBufferCreateInfo vertBufferInfo = {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = 100000
    };
    auto vertexBuffer=SDL_CreateGPUBuffer(device, &vertBufferInfo);
    //上传顶点
    uploadToBuffer(device,vertexBuffer,0,(void*)vertices.data(),vertices.size()*sizeof(ColorVertex3D));
    return vertexBuffer;
}
static SDL_GPUBuffer *uploadIndexToNewBuffer(SDL_GPUDevice *device, std::vector<Indice> indices) {

    //创建索引缓冲区
    SDL_GPUBufferCreateInfo indexBufferInfo = {
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = 100000
    };
    auto indexBuffer=SDL_CreateGPUBuffer(device, &indexBufferInfo);
    //上传索引
    uploadToBuffer(device,indexBuffer,0,(void*)indices.data(),indices.size()*sizeof(Indice));
    return indexBuffer;
}
Render3DPass::Render3DPass(SDL_GPUDevice *device, SDL_Window *window):device(device),window(window) {
    pipeline3d=define3DPipeline(device, window);
}

#include "../../util/TransformSceneUtil.h"
#include "../../Util.h"
#include "../../../gpu-render/RenderComponents.h"
void Render3DPass::uploadData(ecs::Scene *scene) {

    std::vector<ColorVertex3D> vertices = {};
    std::vector<Indice> indices = {};
    //从场景中获取所有顶点和索引
    // for (const auto entity:ecs::getEntities<Drawable3DFlag>(scene)) {
    //     auto* drawableFlag= ecs::getComponent<Drawable3DFlag>(scene, entity);
    //     //上传顶点
    //     renderContext->vertOffsets[entity]=gpu->vert_buffers[this->vert_buffer_index]->vertices.size();
    //     renderContext->vertCounts[entity]=drawableFlag->mesh.vertices.size();
    //     for (const auto & vertice : drawableFlag->mesh.vertices) {
    //         gpu->vert_buffers[this->vert_buffer_index]->vertices.push_back(vertice);
    //     }
    //     //上传索引
    //     renderContext->indexOffsets[entity]=gpu->index_buffers[this->index_buffer_index]->indices.size();
    //     renderContext->indexCounts[entity]=drawableFlag->mesh.indices.size();
    //     for (const auto & index : drawableFlag->mesh.indices) {
    //         gpu->index_buffers[this->index_buffer_index]->indices.push_back(index);
    //     }
    // }
    // dataState.vertexBuffer = uploadColor3DVertexToNewBuffer(device, vertices);
    // dataState.indexBuffer = uploadIndexToNewBuffer(device, indices);
    // dataState.numIndices = indices.size() * 3;
}
static MVP collectMVP(ecs::Scene* scene,Entity entity,Entity camera) {
    auto drawableFlag= ecs::getComponent<Drawable3DFlag>(scene, entity);
    auto transformComp = ecs::getComponent<Transform3DComp>(scene, entity);
    auto cameraComp = ecs::getComponent<Camera3DComp>(scene, camera);
    auto cameraTransformComp=ecs::getComponent<Transform3DComp>(scene, camera);
    assert(transformComp!=nullptr&&drawableFlag!=nullptr&&cameraComp!=nullptr&&cameraTransformComp!=nullptr);
    auto cameraWorldTransform=TransformSceneUtil::computeLocalToWorldTransform(*cameraTransformComp,scene);
    auto worldTransform = TransformSceneUtil::computeLocalToWorldTransform(*transformComp,scene);
    auto modelMatrix=Transform3DUtil::transformToMatrix(worldTransform);
    auto viewMatrix = Transform3DUtil::getReverseTransformToMatrix(cameraWorldTransform);
    auto projectMatrix=Transform3DUtil::matProject(cameraComp->fov,cameraComp->a,cameraComp->nearZ,cameraComp->farZ);
    MVP mvpMatrix={modelMatrix,viewMatrix,projectMatrix};
    return mvpMatrix;
}
void Render3DPass::draw(ecs::Scene *scene,SDL_GPUTexture* targetTexture,SDL_GPUCommandBuffer* commandBuffer,std::unordered_map<Entity,DrawableGPUMesh>drawableMeshes) {
    //目前只拿第一个摄像机
    if (ecs::searchEntity<Camera3DComp>(scene).empty())return;
    Entity camera=ecs::searchEntity<Camera3DComp>(scene)[0];

    // 定义颜色目标，渲染过程颜色目标 = “这次实际渲染要写到哪个纹理，怎么清空和保存”
    SDL_GPUColorTargetInfo colorTargetInfo = {
        .texture = targetTexture,
        .clear_color = { 0.0f, 0.0f, 0.0f, 1.0f },
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };
    // 启动渲染过程
    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);
    for (auto entity:ecs::getEntities<Drawable3DFlag>(scene)) {
        // 绑定渲染管线
        SDL_BindGPUGraphicsPipeline(renderPass, pipeline3d);

        // 绑定顶点缓冲区
        SDL_GPUBufferBinding vboBinding = {
            .buffer = drawableMeshes[entity].vertexBuffer,
            .offset = 0,
        };
        SDL_BindGPUVertexBuffers(renderPass, 0, &vboBinding, 1);
        //绑定索引缓冲区
        SDL_GPUBufferBinding iboBinding = {
            .buffer = drawableMeshes[entity].indexBuffer,
            .offset = 0,
        };
        SDL_BindGPUIndexBuffer(renderPass, &iboBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
        //视口
        SDL_GPUViewport viewport = {
            .w=500,
            .h=500,
        };
        SDL_SetGPUViewport(renderPass,&viewport);

        MVP mvp=collectMVP(scene,entity,camera);//todo 临时变量会不会提交前被销毁？
        SDL_PushGPUVertexUniformData(commandBuffer, 0, &mvp, sizeof(MVP));
        // 三角形绘制: 绘制图元，一个索引结构体对应3个数字
        SDL_DrawGPUIndexedPrimitives(renderPass,drawableMeshes[entity].indexCount*3, 1, 0, 0, 0);
    }
    // 结束渲染过程
    SDL_EndGPURenderPass(renderPass);

}
