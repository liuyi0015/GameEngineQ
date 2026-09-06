//
// Created by abc17 on 2026/8/20.
//

#ifndef GAMEENGINEQ_SOFTRENDERER_H
#define GAMEENGINEQ_SOFTRENDERER_H
#include <string>

#include "glm/glm.hpp"
#include "glm/detail/type_vec3.hpp"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"



class ColorBuffer{
public:
    std::vector<glm::vec4>data;
    int width,height;
    ColorBuffer(int w,int h):width(w),height(h),data(w*h){}
    ColorBuffer(SDL_Surface* surface):width(surface->w),height(surface->h),data(surface->w*surface->h){
        uint8_t* pixels=(uint8_t*)surface->pixels;
        for (int i=0;i<surface->w;i++) {
            for (int j=0;j<surface->h;j++) {
                uint8_t* pixel=pixels+j*surface->pitch+i*4;
                at(i,j)[0]=pixel[0]/255.0f;
                at(i,j)[1]=pixel[1]/255.0f;
                at(i,j)[2]=pixel[2]/255.0f;
                at(i,j)[3]=pixel[3]/255.0f;
            }
        }
    }
    [[nodiscard]] glm::vec4 at(int x,int y) const{
        return data[y*width+x];
    }
    std::vector<glm::vec4> copyData() {
        return data;
    }
    void clear(const glm::vec4 color={0,0,0,1}) {
        std::fill(data.begin(),data.end(),color);
    }
    [[nodiscard]] SDL_Surface* toSurface() const {
        SDL_Surface* surface = SDL_CreateSurface(width,height,SDL_PIXELFORMAT_RGBA8888);
        auto pixels=static_cast<uint8_t *>(surface->pixels);
        for (int i=0;i<width;i++) {
            for (int j=0;j<height;j++) {
                //RGBA8888格式的布局
                pixels[j*width*4+i*4+0]=static_cast<uint8_t>(at(i, j).r)*255.0f;
                pixels[j*width*4+i*4+1]=static_cast<uint8_t>(at(i, j).g)*255.0f;
                pixels[j*width*4+i*4+2]=static_cast<uint8_t>(at(i, j).b)*255.0f;
                pixels[j*width*4+i*4+3]=255;//舍弃透明度，不舍则static_cast<uint8_t>(at(i, j).a)*255.0f;
            }
        }
        return surface;
    }
};
class IPipeline {
protected:
    virtual ~IPipeline() = default;
    virtual void* vertexShader(void* in, const void *uniform)=0;
    virtual glm::vec4 fragmentShader(void* in, const void *uniform)=0;
public:
//三角形相互独立，索引需要有序，底部先画的在前面
    void run(const std::vector<void*> &verts, const std::vector<glm::ivec3> &indices,
             const void *uniform, ::ColorBuffer *target);
};
class RenderPass {
public:
    //用字符串引用方便跨类
    std::string cur_pipeline_name;
    std::string cur_target_name;
};
/**
 * 目前默认格式RGBA8888
 * 目前只有一个顶点和索引缓冲区
 * 只画三角形，每个独立占3顶点
 * 总是使用索引绘制
 */
class SoftGPU {
private:
    SDL_Window* window;
public:
    SoftGPU(SDL_Window* window):window(window){
        SDL_SetWindowSurfaceVSync(window,1);
        int w = 0, h = 0;
        SDL_GetWindowSize(window, &w, &h);
        swapchain_texture=new ColorBuffer(w,h);
    };
    ColorBuffer* swapchain_texture;
    std::unordered_map<std::string,ColorBuffer*> render_targets;
    std::unordered_map<std::string,IPipeline*> pipelines;
    std::vector<void*> vert_buffer;
    std::vector<glm::ivec3> index_buffer;
    RenderPass cur_renderpass;
    //每个drawcall即每个物体的uniform都不同，即使是类型不同
    //uniform的具体类型或者说数据布局只有着色器内部知道
    void drawcall(unsigned long long triangle_offset, unsigned long long triangle_count, unsigned long long vert_offset, void *uniform);
    void present();
};

#endif //GAMEENGINEQ_SOFTRENDERER_H
