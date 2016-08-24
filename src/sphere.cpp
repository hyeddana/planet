#include "sphere.h"

Sphere::Sphere(int numRows, float m_radius) :
    m_numRows(numRows),
    m_numCols(numRows),
    m_radius(m_radius)
{
    m_lat = M_PI / m_numCols;
    m_long = (2 * M_PI) / m_numRows;
    m_top = glm::vec3(0.f, m_radius, 0.f);
    m_bottom = glm::vec3(0.f, -m_radius, 0.f);
}

Sphere::~Sphere()
{

}

/**
 * Returns the object-space position for the terrain vertex at the given row and column.
 */
glm::vec3 Sphere::getPosition(float lat, float lon)
{
    glm::vec3 position = glm::vec3(m_radius * sin(lat) * cos(lon), m_radius * cos(lat), m_radius * sin(lat) * sin(lon));
    return position;
}


/**
 * Returns the normal vector for the terrain vertex at the given row and column.
 */
glm::vec3 Sphere::getNormal(float lat, float lon)
{
    glm::vec3 curr = getPosition(lat, lon);


    // This is a hacky way of fixing the normals at the top
    float latMax = fmax(FLT_EPSILON, lat + m_lat);
    float latMin = fmax(FLT_EPSILON, lat - m_lat);

    glm::vec3 n0 = curr - getPosition(lat, lon + m_long);
    glm::vec3 n1 = curr - getPosition(latMax, lon + m_long);
    glm::vec3 n2 = curr - getPosition(latMax, lon);
    glm::vec3 n3 = curr - getPosition(latMax, lon - m_long);
    glm::vec3 n4 = curr - getPosition(lat, lon - m_long);
    glm::vec3 n5 = curr - getPosition(latMin, lon - m_long);
    glm::vec3 n6 = curr - getPosition(latMin, lon);
    glm::vec3 n7 = curr - getPosition(latMin, lon - m_long);

    glm::vec3 norm1 = glm::normalize(glm::cross(n0, n1));
    glm::vec3 norm2 = glm::normalize(glm::cross(n1, n2));
    glm::vec3 norm3 = glm::normalize(glm::cross(n2, n3));
    glm::vec3 norm4 = glm::normalize(glm::cross(n3, n4));
    glm::vec3 norm5 = glm::normalize(glm::cross(n4, n5));
    glm::vec3 norm6 = glm::normalize(glm::cross(n5, n6));
    glm::vec3 norm7 = glm::normalize(glm::cross(n6, n7));
    glm::vec3 norm8 = glm::normalize(glm::cross(n7, n0));
    return glm::normalize(norm1 + norm2 + norm3 + norm4 + norm5 + norm6 + norm7 + norm8);
}

void Sphere::draw()
{
    m_shape.draw();
}

glm::vec2 Sphere::findUV(glm::vec3 p) {
    glm::vec2 uv = glm::vec2(0.f);
    float phi = asin(p.y/m_radius);
    uv.y = phi/ M_PI + m_radius;
    if (uv.y == 0.f || uv.y == 1.f) {
        uv.x = m_radius;
    } else {
        float theta = atan2(p.z, p.x);

        if (theta < 0.f) {
            uv.x = -theta / (2.f * M_PI);
        } else {
            uv.x = 1.f - (theta / (2.f * M_PI));
        }
    }
    return uv;
}


void Sphere::insertVec3(glm::vec3 vec3ToInsert)
{
    m_data.push_back(vec3ToInsert.x);
    m_data.push_back(vec3ToInsert.y);
    m_data.push_back(vec3ToInsert.z);
}

void Sphere::insertVec2(glm::vec2 vec2ToInsert)
{
    m_data.push_back(vec2ToInsert.x);
    m_data.push_back(vec2ToInsert.y);
}
