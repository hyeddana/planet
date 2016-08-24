#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class objloader
{
public:
    objloader();

    static bool loadOBJ(
        std::string path,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec2> & out_uvs,
        std::vector<glm::vec3> & out_normals
    );
};

#endif // OBJLOADER_H
