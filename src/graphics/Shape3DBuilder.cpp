//
// Created by abc17 on 2026/9/12.
//

#include "Shape3DBuilder.h"

Mesh2D Shape3DBuilder::createCube(glm::vec3 pivot,float edge) {
    Mesh2D mesh;
    //先左右x再上下y最后前后z
    glm::vec3 v[8]={
        {-1,-1,-1},//左下
        {1,-1, -1},//右下
        {-1,1, -1},//左上
        {1,1, -1},//右上
         {-1,-1, 1},
        {1,-1, 1},
        {-1,1, 1},
        {1,1, 1}
    };
    for (int i=0;i<8;i++) {
        v[i]=v[i]*(edge/2)+pivot;
        mesh.points.push_back(v[i]);
    }
    //从外面看顺时针
    //前面
    mesh.indices.push_back({0,2,3});
    mesh.indices.push_back({0,3,1});
    //后面todo
    mesh.indices.push_back({4,6,7});
    mesh.indices.push_back({4,7,5});
    //上面
    mesh.indices.push_back({2,6,7});
    mesh.indices.push_back({2,7,3});
    //下面
    mesh.indices.push_back({0,4,5});
    mesh.indices.push_back({0,5,1});
    //左面
    mesh.indices.push_back({4,6,2});
    mesh.indices.push_back({4,2,0});
    //右面
    mesh.indices.push_back({0,2,3});
    mesh.indices.push_back({0,2,3});

    return mesh;
}
