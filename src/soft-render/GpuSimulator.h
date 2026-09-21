//
// Created by abc17 on 2026/8/20.
//

#ifndef GAMEENGINEQ_SOFTRENDERER_H
#define GAMEENGINEQ_SOFTRENDERER_H
#include <algorithm>
#include <any>
#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "glm/detail/type_vec3.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"


class ColorBuffer{
public:
    std::vector<glm::vec4>data;
    int width,height;
    ColorBuffer(int w,int h):width(w),height(h),data(w*h){}
    //surface格式可以随便，不一定raba8888，以便支持各种图片
    ColorBuffer(SDL_Surface* surface):width(surface->w),height(surface->h),data(surface->w*surface->h){
        printf("surface format: %s\n", SDL_GetPixelFormatName(surface->format));
        // SDL_LockSurface(surface);
        uint8_t r,g,b,a;
        for (int j=0;j<surface->h;j++){
            uint8_t* row = static_cast<uint8_t*>(surface->pixels) + j * surface->pitch;
            for (int i=0;i<surface->w;i++){
                uint32_t pixel = *reinterpret_cast<uint32_t*>(row + i*4);
                SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(surface->format),NULL, &r, &g, &b, &a);
                glm::vec4 color = {
                    r / 255.0f,
                    g / 255.0f,
                    b / 255.0f,
                    a / 255.0f
                };
                set(i, j, color);
            }
        }
        // SDL_UnlockSurface(surface);
    }
    [[nodiscard]] glm::vec4 at(int x,int y) const{
        //随便处理一下边界
        if (x>=width) x=width-1;
        if (y>=height) y=height-1;
        if (x<0) x=0;
        if (y<0) y=0;
        return data[y*width+x];
    }
    void set(int x,int y,glm::vec4 color) {
        data[y*width+x]=color;
    }
    void clear(const glm::vec4 color={0,0,0,1}) {
        std::fill(data.begin(),data.end(),color);
    }
    [[nodiscard]] SDL_Surface* toSurface(SDL_PixelFormat format=SDL_PIXELFORMAT_RGBA8888) const {
        SDL_Surface* surface = SDL_CreateSurface(width,height,format);
        for (int i=0;i<width;i++) {
            for (int j=0;j<height;j++) {
                glm::vec4 c = at(i,j);
                uint8_t rr = static_cast<uint8_t>(std::clamp(c.r,0.0f,1.0f) * 255.0f);
                uint8_t gg = static_cast<uint8_t>(std::clamp(c.g,0.0f,1.0f) * 255.0f);
                uint8_t bb = static_cast<uint8_t>(std::clamp(c.b,0.0f,1.0f) * 255.0f);
                uint8_t aa = static_cast<uint8_t>(std::clamp(c.a,0.0f,1.0f) * 255.0f);
                uint32_t px = SDL_MapSurfaceRGBA(surface, rr, gg, bb, aa);
                uint8_t* row = static_cast<uint8_t*>(surface->pixels) + j * surface->pitch;
                *reinterpret_cast<uint32_t*>(row + i*4) = px;
            }
        }
        return surface;
    }
    void testColorBuffer() {
        SDL_Window* window=SDL_CreateWindow("ColorBuffer Test", 800, 600,0);
        SDL_Renderer* renderer=SDL_CreateRenderer(window,"opengl");
        auto img1=this->toSurface();
        SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer, img1);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer,texture,nullptr,nullptr);
        SDL_RenderPresent(renderer);
        // while (true);
    }
};

struct Uniform {//一次绘制中全局不变的数据
    //虚析构才能调用子类的析构
    virtual ~Uniform() = default;
};

struct VertexShaderOutput {
    glm::vec4 pos;//x,y,深度,插值后像素的1/w
    glm::vec2 uv;
    glm::vec4 color;
};
struct FragmentAttrib {
    glm::vec4 viewPos;
    glm::vec4 color;
    glm::vec2 uv;
};

class IShader {
public:
    virtual ~IShader() = default;
    virtual VertexShaderOutput* vertexShader(const std::any& in, const Uniform* uniform)=0;
    virtual glm::vec4 fragmentShader(const FragmentAttrib* in, const Uniform* uniform)=0;
};
struct VertexBuffer{
    std::vector<std::any> vertices;
};
struct IndiceBuffer {
    std::vector<glm::ivec3> indices;
};
struct UniformBuffer {
    std::vector<Uniform*> uniforms;
};
struct  RenderPass {
public:
    ColorBuffer* const cur_target;
    const unsigned long long vert_buffer_offset;
    const unsigned long long index_buffer_offset;
    const unsigned long long uniform_buffer_offset;
    IShader* cur_shader=nullptr;
    RenderPass(ColorBuffer* target,unsigned long long vert_buffer_offset,unsigned long long index_buffer_offset,
        unsigned long long uniform_buffer_offset):cur_target(target),vert_buffer_offset(vert_buffer_offset),
    index_buffer_offset(index_buffer_offset),uniform_buffer_offset(uniform_buffer_offset){};
};
/**
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
    std::unordered_map<std::string,IShader*> pipelines;
    std::vector<VertexBuffer*> vert_buffers;
    std::vector<IndiceBuffer*> index_buffers;
    std::vector<UniformBuffer*>uniform_buffers;
    void drawcall(const RenderPass &render_pass, unsigned long long triangle_offset, unsigned long long triangle_count,
                  unsigned long long vert_offset, unsigned long long uniform_offset);
    void present();
};

#endif //GAMEENGINEQ_SOFTRENDERER_H
