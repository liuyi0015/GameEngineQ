//
// Created by abc17 on 2026/8/20.
//

#include "GpuSimulator.h"

#include "glm/glm.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "../graphics/Shape2DBuilder.h"
#include "../transform/transform3d/Transform3DUtil.h"

//深度缓存
static float depth[10000][10000];

void SoftGPU::BeginRenderPass(RenderPass render_pass) {
    cur_render_pass=render_pass;
    ColorBuffer* target=render_pass.cur_target;
    target->clear({0,0,0,1});
    //重置深度
    for (int i=0;i<target->width;i++) {
        for (int j=0;j<target->height;j++) {
            depth[i][j]=1;//初始化为最大深度
        }
    }
}
static void runPipeline(const std::vector<std::any>& verts, const std::vector<glm::ivec3> &indices,
                        const Uniform *uniform,ColorBuffer* const target,IShader* shader,bool edgeMode) {
    //顶点着色
    VertexShaderOutput* vert_outs[verts.size()];
    for (int i = 0; i < verts.size(); ++i) {
        vert_outs[i] = shader->vertexShader(verts[i],uniform);
    }
    //光栅化
    //渲染管线可能在绘制中途工作，不能清空渲染目标

    for (int k=0;k<indices.size();k++) {
        auto vert_out0=*vert_outs[indices[k][0]];
        auto vert_out1=*vert_outs[indices[k][1]];
        auto vert_out2=*vert_outs[indices[k][2]];
        //顶点视口变换，除了缩放，还要转换为屏幕坐标系，忽略z值
        glm::vec2 t0=vert_out0.pos*Transform3DUtil::matViewport(target->width*1.0f,target->height*1.0f);
        glm::vec2 t1=vert_out1.pos*Transform3DUtil::matViewport(target->width*1.0f,target->height*1.0f);
        glm::vec2 t2=vert_out2.pos*Transform3DUtil::matViewport(target->width*1.0f,target->height*1.0f);
        PointTriangle2D triangle{t0,t1,t2};
        for (int i=0;i<target->width;i++) {
            for (int j=0;j<target->height;j++) {
                //坐标统统加0.5，才是像素中心
                glm::vec2 screenPos={i+0.5,j+0.5};
                if (triangle.contains(screenPos) ){
                    FragmentAttrib frag{};
                    frag.viewPos={screenPos,0,1};
                    //面积比表示重心坐标
                    PointTriangle2D triangle0{screenPos,t1,t2};
                    PointTriangle2D triangle1{screenPos,t0,t2};
                    PointTriangle2D triangle2{screenPos,t0,t1};
                    float w0=triangle0.area()/triangle.area();
                    float w1=triangle1.area()/triangle.area();
                    float w2=triangle2.area()/triangle.area();
                    //深度测试
                    float deep=w0*vert_out0.pos.z+w1*vert_out1.pos.z+w2*vert_out2.pos.z;
                    if (deep<depth[i][j]) {
                        depth[i][j]=deep;
                    }else {
                        continue;
                    }
                    if (edgeMode) {

                        // 只保留边缘：任一重心坐标很接近 0，就认为在边上
                        bool isEdge = (w0 < 0.01f || w1 < 0.01f || w2 < 0.02f);

                        if (isEdge) {
                            target->set(i, j, {0, 1, 1, 1}); // 线框颜色
                        }else {
                            target->set(i, j, {0, 0, 0, 1});
                        }
                    }else {

                    //颜色插值
                    frag.color=w0*vert_out0.color+w1*vert_out1.color+w2*vert_out2.color;
                    //uv插值
                    frag.uv=w0*vert_out0.uv+w1*vert_out1.uv+w2*vert_out2.uv;

                    target->set(i,j,shader->fragmentShader(&frag,uniform));
                    }
                }
            }
        }
    }
    // target->testColorBuffer();
}

void SoftGPU::drawcall(unsigned long long triangle_offset, unsigned long long triangle_count,
    unsigned long long vert_offset,unsigned long long uniform_offset) {
    //每次都从0开始覆盖，每帧都要重新上传
    //顶点索引偏移
    std::vector<glm::ivec3> triangles;
    for (int i=triangle_offset;i<triangle_offset+triangle_count;i++) {
        auto value = index_buffers[cur_render_pass.index_buffer_offset]->indices[i];
        glm::ivec3 triangle={value.x+vert_offset,value.y+vert_offset,value.z+vert_offset};
        triangles.push_back(triangle);
    }
    IShader* shader=cur_render_pass.cur_shader;
    Uniform* uniform=uniform_buffers[cur_render_pass.uniform_buffer_offset]->uniforms[uniform_offset];
    ColorBuffer* target=cur_render_pass.cur_target;
    runPipeline(vert_buffers[cur_render_pass.vert_buffer_offset]->vertices,triangles,uniform,target,shader,cur_render_pass.edgeMode);
    // triangle_offset+=triangle_count;
}


void SoftGPU::present() {
    // for (int i=0;i<swapchain_texture->width;i++) {
    //     for (int j=0;j<swapchain_texture->height;j++) {
    //         if (swapchain_texture->at(i,j).g>0) {
    //             printf("g");
    //         }
    //     }
    // }
    SDL_Surface* back_surface=swapchain_texture->toSurface();
    auto winsur = SDL_GetWindowSurface(window);
    //据说从显存来的surface要锁定才能正常访问像素数据，其实锁了反而显示不了
    // SDL_LockSurface(winsur);
    SDL_FillSurfaceRect(winsur, nullptr, SDL_MapSurfaceRGB(winsur, 0, 0, 0));
    SDL_BlitSurface(back_surface,NULL,winsur,NULL);
    SDL_UpdateWindowSurface(window);
    // SDL_UnlockSurface(winsur);
    SDL_DestroySurface(back_surface);
    swapchain_texture->clear();

}
