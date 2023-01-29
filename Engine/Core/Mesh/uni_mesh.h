#pragma once

#include "mesh.h"

class UniMesh
{
public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> normals;
    std::vector<glm::vec3> color;
    std::vector<glm::vec2> texture_st0;
    std::vector<glm::vec2> texture_st1;
    
    std::vector<glm::vec3> positions2d;
    std::vector<glm::vec4> color4;

    std::vector<uint32_t> indices;
    Mesh::Primitive mode;
};