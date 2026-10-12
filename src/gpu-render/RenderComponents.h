//
// Created by XL0002 on 2026/10/12.
//

#ifndef GAMEENGINEQ_RENDERCOMPONENTS_H
#define GAMEENGINEQ_RENDERCOMPONENTS_H
struct Mesh {
    std::vector<void*> vertices;
    std::vector<glm::ivec3> indices;
};
//通道相关
struct Camera2DComp {
    float captureWidth;
    float captureHeight;
    // ColorBuffer* target;
};
//绘制声明，和资源声明互不干扰
struct Drawable2DFlag {
    int z_order = 0;
    void* material;
    Mesh* mesh;
};
struct Camera3DComp {
    //垂直视场角
    float fov;
    //视锥宽高比
    float a;
    float nearZ;
    float farZ;
    // ColorBuffer* target;
};
struct Drawable3DFlag {
    void* material;
    Mesh mesh;
};


#endif //GAMEENGINEQ_RENDERCOMPONENTS_H
