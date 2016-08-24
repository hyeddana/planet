#ifndef TERRAIN_H
#define TERRAIN_H

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr
#include <vector>

#include "openglshape.h"

#include "sphere.h"

// Include perlin code.
#include "perlin/PerlinNoise.h"
#include <iostream>

#include <QHash>

#include <map>

#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <memory>

class Terrain
        : public Sphere
{
public:
    Terrain(float frequency, float persistence, float amplitude, int octave);

    void init();

    float m_minHeight, m_maxHeight;

    // Returns min and max height of terrain (used as a range when computing terrain coloring)
    glm::vec2 getMinMaxHeight();

private:

    // Perlin noies stuff. (might not need member variable)
    std::unique_ptr<PerlinNoise> m_perlinGen;

    // Specify a section that you want to apply the perling noise too. Also
    // inserts each extruded vertex into our hashmap of verticies (used to
    // calculated new, extruded normals).
    void specifySection(int startRow, int startCol, int endRow, int endCol);

    // Hashmap of hashmaps used if a vertex is already defined
    std::map<float, std::map<float, glm::vec3>> m_latLongMap;

    // Function that takes in a given col and row and inserts the extruded point into
    // our hashmap tied to that coordinate.
    void insertExtrudedVertex(int col, int row, glm::vec3 extrudedPt);

    // Takes in a latitude, longitude, and the current point and returns the normal of the extruded vertex
    // at that point (taking into account the extruded neighbors).
    glm::vec3 getExistExtrudedNormal(int col, int row);

    // Inserts proper data per point into vector used in VAO.
    // (inserts xyz coordinates, normals, uv coordinates, and height (perlin noise)
    // tied to each coordinate). Calculates min/max heights during terrain generation,
    // and sets m_minHeight and m_maxHeight values to be returned.
    void insertFinalPoints();
};

#endif // TERRAIN_H
