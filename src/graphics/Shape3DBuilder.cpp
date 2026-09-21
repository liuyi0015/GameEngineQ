//
// Created by abc17 on 2026/9/12.
//

#include "Shape3DBuilder.h"

Shape3D Shape3DBuilder::createCube(glm::vec3 pivot,float edgeLong) {
    Shape3D mesh;
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
        v[i]=v[i]*(edgeLong/2)+pivot;
        mesh.points.push_back(v[i]);
    }
    //从外面看顺时针
    //前面
    mesh.indices.push_back({0,2,3});
    mesh.indices.push_back({0,3,1});
    //后面
    mesh.indices.push_back({5,7,6});
    mesh.indices.push_back({5,6,4});
    //上面
    mesh.indices.push_back({2,6,7});
    mesh.indices.push_back({2,7,3});
    //下面
    mesh.indices.push_back({4,0,1});
    mesh.indices.push_back({4,1,5});
    //左面
    mesh.indices.push_back({4,6,2});
    mesh.indices.push_back({4,2,0});
    //右面
    mesh.indices.push_back({1,3,7});
    mesh.indices.push_back({1,7,5});

    return mesh;
}
