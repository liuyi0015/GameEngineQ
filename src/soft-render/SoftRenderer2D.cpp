//
// Created by abc17 on 2026/8/20.
//

#include "SoftRenderer2D.h"

#include "../demo/render-blueprint/Pipelines.h"
#include "glm/glm.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "../graphics/MeshBuilder.h"


void IPipeline::run(const std::vector<void*> &verts, const std::vector<glm::ivec3> &indices,
    const void *uniform,ColorBuffer* const target) {
    //顶点着色
    void* vert_outs[verts.size()];
    for (int i = 0; i < verts.size(); ++i) {
        vert_outs[i] = vertexShader(verts[i],uniform);
    }
    //光栅化，透明度还要留着，合成最终纹理之后才能丢
    //渲染管线可能在中途工作，不能清空渲染目标
    for (int k=0;k<indices.size();k++) {
        auto vert_out0=*(VertexAttrib*)vert_outs[indices[k][0]];
        auto vert_out1=*(VertexAttrib*)vert_outs[indices[k][1]];
        auto vert_out2=*(VertexAttrib*)vert_outs[indices[k][2]];
        Mesh::PointTriangle2D triangle{vert_out0.pos,vert_out1.pos,vert_out2.pos};
        //先宽后高
        for (int i=0;i<target->width;i++) {
            for (int j=0;j<target->height;j++) {
                //顶点视口变换
                glm::vec2 t0={vert_out0.pos.x*target->width,vert_out0.pos.y*target->height};
                glm::vec2 t1={vert_out1.pos.x*target->width,vert_out1.pos.y*target->height};
                glm::vec2 t2={vert_out2.pos.x*target->width,vert_out2.pos.y*target->height};
                //坐标统统加0.5，才是像素中心
                glm::vec2 pos={i+0.5,j+0.5};
                if (triangle.contains(pos) ){
                    FragmentAttrib frag{};
                    frag.viewPos=pos;
                    //面积比表示重心坐标
                    Mesh::PointTriangle2D triangle0{pos,t0,t1};
                    Mesh::PointTriangle2D triangle2{pos,t1,t2};
                    Mesh::PointTriangle2D triangle1{pos,t2,t0};
                    //颜色插值
                    float r0=vert_out0.color.r;
                    float r1=vert_out1.color.r;
                    float r2=vert_out2.color.r;
                    frag.color.r=(r0*triangle0.area()+r1*triangle1.area()+r2*triangle2.area())/triangle.area();
                    float g0=vert_out0.color.g;
                    float g1=vert_out1.color.g;
                    float g2=vert_out2.color.g;
                    frag.color.g=(g0*triangle0.area()+g1*triangle1.area()+g2*triangle2.area())/triangle.area();
                    float b0=vert_out0.color.b;
                    float b1=vert_out1.color.b;
                    float b2=vert_out2.color.b;
                    frag.color.b=(b0*triangle0.area()+b1*triangle1.area()+b2*triangle2.area())/triangle.area();
                    float a0=vert_out0.color.a;
                    float a1=vert_out1.color.a;
                    float a2=vert_out2.color.a;
                    frag.color.a=(a0*triangle0.area()+a1*triangle1.area()+a2*triangle2.area())/triangle.area();
                    //uv插值
                    float u0=vert_out0.uv.x;
                    float u1=vert_out1.uv.x;
                    float u2=vert_out2.uv.x;
                    float v0=vert_out0.uv.y;
                    float v1=vert_out1.uv.y;
                    float v2=vert_out2.uv.y;
                    frag.uv.x=(u0*triangle0.area()+u1*triangle1.area()+u2*triangle2.area())/triangle.area();
                    frag.uv.y=(v0*triangle0.area()+v1*triangle1.area()+v2*triangle2.area())/triangle.area();
                    target->at(i,j)=fragmentShader(&frag,uniform);
                }
            }
        }
    }
}

void SoftGPU::drawcall( unsigned long long triangle_offset, unsigned long long triangle_count,
    unsigned long long vert_offset,void* uniform) {
    if (triangle_offset+triangle_count>index_buffer.size()) {
        triangle_offset=0;
    }
    //偏移定位
    std::vector<glm::ivec3> triangles;
    for (int i=triangle_offset;i<triangle_offset+triangle_count;i++) {
        glm::ivec3 triangle={index_buffer[i].x+vert_offset,index_buffer[i].y+vert_offset,index_buffer[i].z+vert_offset};
        triangles.push_back(triangle);
    }
    ColorBuffer* curTarget=render_targets[cur_renderpass.cur_target_name];
    IPipeline* pipeline=pipelines[cur_renderpass.cur_pipeline_name];
    pipeline->run(vert_buffer,triangles,uniform,curTarget);
    // triangle_offset+=triangle_count;
}

void SoftGPU::present() {
    SDL_Surface* back_surface=swapchain_texture->toSurface();
    auto winsur = SDL_GetWindowSurface(window);
    SDL_BlitSurface(back_surface,NULL,winsur,NULL);
    SDL_UpdateWindowSurface(window);
    SDL_DestroySurface(back_surface);
}
