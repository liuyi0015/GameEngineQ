//
// Created by XL0002 on 2026/10/15.
//

#include "Upload3DPass.h"
#include "../../Util.h"
#include "../../comps/RenderComponents.h"

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

static SDL_GPUBuffer *uploadColor3DVertexToNewBuffer(SDL_GPUDevice *device,const std::vector<std::any>& vertices) {
    //解包，构造Color3DVertex数组（修改内存布局）
    std::vector<ColorVertex3D> tmp;
    tmp.reserve(vertices.size());
    for (auto &a : vertices) {
        tmp.push_back(std::any_cast<ColorVertex3D>(a));
    }
    //创建顶点缓冲区
    SDL_GPUBufferCreateInfo vertBufferInfo = {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = 100000
    };
    auto vertexBuffer=SDL_CreateGPUBuffer(device, &vertBufferInfo);
    //上传顶点
    uploadToBuffer(device,vertexBuffer,0,(void*)tmp.data(),tmp.size()*sizeof(ColorVertex3D));
    return vertexBuffer;
}
static SDL_GPUBuffer *uploadIndexToNewBuffer(SDL_GPUDevice *device, const std::vector<Indice>& indices) {

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

void Upload3DPass::uploadData(ecs::Scene *scene,std::unordered_map<Entity, DrawableGPUMesh>& drawableMeshes) {
    size_t offset1 = 0, offset2 = 0;
    for (const Entity entity:ecs::getEntities<Drawable3DFlag>(scene)) {
        auto* drawableFlag= ecs::getComponent<Drawable3DFlag>(scene, entity);
        drawableMeshes.insert({entity, DrawableGPUMesh{}});
        //上传顶点
        drawableMeshes.at(entity).vertexBuffer = uploadColor3DVertexToNewBuffer(device, drawableFlag->mesh.vertices);
        drawableMeshes.at(entity).vertexCount=drawableFlag->mesh.vertices.size();
        drawableMeshes.at(entity).vertexOffset=offset1;
        offset1+=drawableFlag->mesh.vertices.size();
        //上传索引
        drawableMeshes.at(entity).indexBuffer = uploadIndexToNewBuffer(device, drawableFlag->mesh.indices);
        drawableMeshes.at(entity).indexCount=drawableFlag->mesh.indices.size()*3;
        drawableMeshes.at(entity).indexOffset=offset2;
        offset2+=drawableFlag->mesh.indices.size()*3;

    }
}