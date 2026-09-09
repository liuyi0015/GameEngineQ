//
// Created by abc17 on 2026/8/20.
//

#ifndef GAMEENGINEQ_SOFTRENDERER_H
#define GAMEENGINEQ_SOFTRENDERER_H
#include <iostream>
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
        // SDL_LockSurface(surface);
        uint8_t r,g,b,a;
        for (int j=0;j<surface->h;j++){
            uint8_t* row = static_cast<uint8_t*>(surface->pixels) + j * surface->pitch;
            for (int i=0;i<surface->w;i++){
                uint32_t pixel = *reinterpret_cast<uint32_t*>(row + i*4);
                SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888),NULL, &r, &g, &b, &a);
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
        return data[y*width+x];
    }
    void set(int x,int y,glm::vec4 color) {
        data[y*width+x]=color;
    }
    void clear(const glm::vec4 color={0,0,0,1}) {
        std::fill(data.begin(),data.end(),color);
    }
    [[nodiscard]] SDL_Surface* toSurface() const {
        SDL_Surface* surface = SDL_CreateSurface(width,height,SDL_PIXELFORMAT_RGBA8888);
        auto pixels=static_cast<uint8_t *>(surface->pixels);
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
};

//软渲染器暂时先把类型写死方便debug
struct Uniform {//一次绘制中全局不变的数据
    glm::mat3 mvpMatrix;//需要投影到ndc坐标
    ColorBuffer* texture;
    // 其他全局数据，比如时间、透明度、混合颜色、光照、高度图等也可以放这里
    ~Uniform() {
        delete texture;
    }
};

struct VertexAttrib {//每个顶点各自的数据
    glm::vec2 pos;// ndc
    glm::vec2 uv;
    glm::vec4 color;
};
struct VertexShaderOutput {
    glm::vec2 pos;
    glm::vec2 uv;
    glm::vec4 color;
};
struct FragmentAttrib {
    glm::vec2 viewPos;
    glm::vec4 color;
    glm::vec2 uv;
};
class IPipeline {
protected:
    virtual ~IPipeline() = default;
    virtual VertexShaderOutput* vertexShader(const VertexAttrib* in, const Uniform* uniform)=0;
    virtual glm::vec4 fragmentShader(const FragmentAttrib* in, const Uniform* uniform)=0;
public:
//三角形相互独立，索引需要有序，底部先画的在前面
    void run(const std::vector<VertexAttrib> &verts, const std::vector<glm::ivec3> &indices,
             const Uniform *uniform, ColorBuffer *target);
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
    std::vector<VertexAttrib> vert_buffer;
    std::vector<glm::ivec3> index_buffer;
    RenderPass cur_renderpass;
    //每个drawcall即每个物体的uniform都不同，即使是类型不同
    //uniform的具体类型或者说数据布局只有着色器内部知道
    void drawcall(unsigned long long triangle_offset, unsigned long long triangle_count, unsigned long long vert_offset, const Uniform *uniform);
    void present();
};

#endif //GAMEENGINEQ_SOFTRENDERER_H
