#ifndef SPHERE_H
#define SPHERE_H

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr
#include <vector>

#include "openglshape.h"

#include <QHash>

// Include perlin code.
#include "perlin/PerlinNoise.h"
#include <iostream>

#include <QHash>

#include <map>

#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

class Sphere
{
public:
    Sphere(int numRows, float radius);

    virtual ~Sphere();

    void draw();

protected:
    float m_lat, m_long, m_radius;

    glm::vec3 m_top, m_bottom;

    const float m_numRows, m_numCols;

    OpenGLShape m_shape;

    std::vector<GLfloat> m_data;

    glm::vec3 getPosition(float lat, float lon);

    glm::vec3 getNormal(float lat, float lon);

    glm::vec2 findUV(glm::vec3 p);

    // Inserts the individual components of a vec3 into m_data.
    void insertVec3(glm::vec3 vec2ToInsert);
    // Inserts the individual components of a vec3 into m_data.
    void insertVec2(glm::vec2 vec2ToInsert);
};

#endif // SPHERE_H
