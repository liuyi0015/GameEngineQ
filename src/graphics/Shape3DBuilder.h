//
// Created by abc17 on 2026/9/12.
//

#ifndef GAMEENGINEQ_MESH3DBUILDER_H
#define GAMEENGINEQ_MESH3DBUILDER_H
#include "glm/vec3.hpp"

struct Shape3D {
    std::vector<glm::vec3> points;
    std::vector<glm::ivec3> indices;
};

class Shape3DBuilder {
public:
    static Shape3D createCube(glm::vec3 pivot={0,0,0}, float edgeLong=1);

};


#endif //GAMEENGINEQ_MESH3DBUILDER_H
