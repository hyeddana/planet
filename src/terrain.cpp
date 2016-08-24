 #include "terrain.h"
#include <typeinfo>
#include <math.h>

Terrain::Terrain(float frequency, float persistence, float amplitude, int octave) :
    Sphere(700, 3.f),
    m_minHeight(0.f),
    m_maxHeight(0.f)
{
    // default values (1, 0.01, 6, 4, 3)
    int random = (rand() % 100) + 1;
    double _persistence = static_cast<double>(persistence);
    double _frequency = static_cast<double>(frequency);
    double _amplitude = static_cast<double>(amplitude);

    m_perlinGen.reset(new PerlinNoise(_persistence, _frequency, _amplitude, octave, random));

    m_lat = M_PI / m_numCols;
    m_long = (2 * M_PI) / m_numRows;
    init();
}


/**
 * Initializes the terrain by storing positions and normals in a vertex buffer.
 */
void Terrain::init()
{
    // TODO: Change from GL_LINE to GL_FILL in order to render full triangles instead of wireframe.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Janky af, change this to actually calculate these values and
    // apply the terrain
    glm::vec2 uvTop = findUV(m_top);
    glm::vec2 uvBottom = findUV(m_bottom);
    float perlinHeightTop = m_perlinGen->GetHeight(uvTop.x * m_numCols, uvTop.y * m_numCols);
    float perlinHeightBottom = m_perlinGen->GetHeight(uvBottom.x * m_numCols, uvBottom.y * m_numCols);


    // Calculate points for caps.
    for (int i = 0; i < m_numRows; i++) {
        glm::vec3 point1 = getPosition(m_lat,  m_long * i);
        glm::vec3 point2 = getPosition(m_lat,  (m_long * i) + m_long);

        glm::vec2 uvPt1 = findUV(point1);
        glm::vec2 uvPt2 = findUV(point2);

        float perlinHeightPt1 = m_perlinGen->GetHeight(uvPt1.x * m_numCols, uvPt1.y * m_numCols);
        float perlinHeightPt2 = m_perlinGen->GetHeight(uvPt2.x * m_numCols, uvPt2.y * m_numCols);

        insertVec3(point2);
        insertVec3(glm::normalize(point2));
        insertVec2(uvPt2);
        m_data.push_back(perlinHeightPt2);

        insertVec3(point1);
        insertVec3(glm::normalize(point1));
        insertVec2(uvPt1);
        m_data.push_back(perlinHeightPt1);

        insertVec3(m_top);
        insertVec3(glm::normalize(m_top));
        insertVec2(uvTop);
        m_data.push_back(perlinHeightTop);
    }
    for (int i = 0; i < m_numRows; i++) {
        glm::vec3 point1 = getPosition(m_lat * (m_numCols - 1),  m_long * i);
        glm::vec3 point2 = getPosition(m_lat * (m_numCols - 1),  (m_long * i) + m_long);

        glm::vec2 uvPt1 = findUV(point1);
        glm::vec2 uvPt2 = findUV(point2);

        float perlinHeightPt1 = m_perlinGen->GetHeight(uvPt1.x * m_numCols, uvPt1.y * m_numCols);
        float perlinHeightPt2 = m_perlinGen->GetHeight(uvPt2.x * m_numCols, uvPt2.y * m_numCols);

        insertVec3(m_bottom);
        insertVec3(glm::normalize(m_bottom));
        insertVec2(uvBottom);
        m_data.push_back(perlinHeightBottom);

        insertVec3(point1);
        insertVec3(glm::normalize(point1));
        insertVec2(uvPt1);
        m_data.push_back(perlinHeightPt1);

        insertVec3(point2);
        insertVec3(glm::normalize(point2));
        insertVec2(uvPt2);
        m_data.push_back(perlinHeightPt2);
    }

    specifySection(0, 1, m_numRows, m_numCols - 1);

    insertFinalPoints();

    int numVertices = m_data.size();

    // Initialize OpenGLShape.
    m_shape.create();
    m_shape.setVertexData(&m_data[0], sizeof(GLfloat) * numVertices, GL_TRIANGLES, numVertices);
    m_shape.setAttribute(0, 3, GL_FLOAT, GL_FALSE, (3*3) * sizeof(GLfloat), 0);
    m_shape.setAttribute(1, 3, GL_FLOAT, GL_FALSE, (3*3) * sizeof(GLfloat), 3 * sizeof(GLfloat));
    m_shape.setAttribute(2, 2, GL_FLOAT, GL_FALSE, (3*3) * sizeof(GLfloat), 6 * sizeof(GLfloat));

    // New attribute that stores current 'height' of vertex (perlin noise level)
    m_shape.setAttribute(3, 1, GL_FLOAT, GL_FALSE, (3*3) * sizeof(GLfloat), 8 * sizeof(GLfloat));
}


void Terrain::specifySection(int startRow, int startCol, int endRow, int endCol)
{
    for (int row = startRow; row < endRow; row++) {
        for (int col = startCol; col < endCol; col++) {
            glm::vec3 pt1 = getPosition(m_lat * col, m_long * row);
            glm::vec3 pt2 = getPosition((m_lat * col) + m_lat, m_long * row);
            glm::vec3 pt3 = getPosition((m_lat * col) + m_lat, (m_long * row) + m_long);
            glm::vec3 pt4 = getPosition((m_lat * col), (m_long * row) + m_long);

            glm::vec3 pt1Norm = getNormal(m_lat * col, m_long * row);
            glm::vec3 pt2Norm = getNormal((m_lat * col) + m_lat, m_long * row);
            glm::vec3 pt3Norm = getNormal((m_lat * col) + m_lat, (m_long * row) + m_long);
            glm::vec3 pt4Norm = getNormal((m_lat * col), (m_long * row) + m_long);

            float bumps;
            glm::vec2 uv;

            float divider = 15.f;

            uv = findUV(pt1);
            uv *= m_numCols;
            bumps = m_perlinGen->GetHeight(uv.x, uv.y);
            glm::vec3 extrudedPt1 = pt1 + ((bumps / divider) * pt1Norm);

            uv = findUV(pt2);
            uv *= m_numCols;
            bumps = m_perlinGen->GetHeight(uv.x, uv.y);
            glm::vec3 extrudedPt2 = pt2 + ((bumps / divider) * pt2Norm);

            uv = findUV(pt3);
            uv *= m_numCols;
            bumps = m_perlinGen->GetHeight(uv.x, uv.y);
            glm::vec3 extrudedPt3 = pt3 + ((bumps / divider) * pt3Norm);

            uv = findUV(pt4);
            uv *= m_numCols;
            bumps = m_perlinGen->GetHeight(uv.x, uv.y);
            glm::vec3 extrudedPt4 = pt4 + ((bumps / divider) * pt4Norm);

            // Check if extruded vertices exists already.
            insertExtrudedVertex(col, row, extrudedPt1);
            insertExtrudedVertex(col + 1, row, extrudedPt2);
            insertExtrudedVertex(col + 1, row + 1, extrudedPt3);
            insertExtrudedVertex(col, row + 1, extrudedPt4);
        }
    }
}

void Terrain::insertFinalPoints() {
    float minHeight, maxHeight = 0.f;

    for (int row = 0; row < m_numRows; row++) {
        for (int col = 1; col < m_numCols - 1; col++) {
            glm::vec3 pt1 = getPosition(m_lat * col, m_long * row);
            glm::vec3 pt2 = getPosition((m_lat * col) + m_lat, m_long * row);
            glm::vec3 pt3 = getPosition((m_lat * col) + m_lat, (m_long * row) + m_long);
            glm::vec3 pt4 = getPosition((m_lat * col), (m_long * row) + m_long);

            glm::vec3 extrudedPt1 = m_latLongMap[col][row];
            glm::vec3 extrudedPt2 = m_latLongMap[col + 1][row];
            glm::vec3 extrudedPt3 = m_latLongMap[col + 1][row + 1];
            glm::vec3 extrudedPt4 = m_latLongMap[col][row + 1];

            glm::vec3 pt1Norm = getExistExtrudedNormal(col, row);
            glm::vec3 pt2Norm = getExistExtrudedNormal(col + 1, row);
            glm::vec3 pt3Norm = getExistExtrudedNormal(col + 1, row + 1);
            glm::vec3 pt4Norm = getExistExtrudedNormal(col, row + 1);

            glm::vec2 uvPt1 = findUV(pt1);
            glm::vec2 uvPt2 = findUV(pt2);
            glm::vec2 uvPt3 = findUV(pt3);
            glm::vec2 uvPt4 = findUV(pt4);

            float perlinHeightPt1 = m_perlinGen->GetHeight(uvPt1.x * m_numCols, uvPt1.y * m_numCols);
            float perlinHeightPt2 = m_perlinGen->GetHeight(uvPt2.x * m_numCols, uvPt2.y * m_numCols);
            float perlinHeightPt3 = m_perlinGen->GetHeight(uvPt3.x * m_numCols, uvPt3.y * m_numCols);
            float perlinHeightPt4 = m_perlinGen->GetHeight(uvPt4.x * m_numCols, uvPt4.y * m_numCols);

//            std::cout<<"perlin height ="<<perlinHeightPt1<<std::endl;

            /** NOTE: insertVec2(etrudedPt) could be returning the incorrect UV coordinate */

            insertVec3(extrudedPt3);
            insertVec3(pt3Norm);
            insertVec2(uvPt3);
            m_data.push_back(perlinHeightPt3);

            insertVec3(extrudedPt2);
            insertVec3(pt2Norm);
            insertVec2(uvPt2);
            m_data.push_back(perlinHeightPt2);

            insertVec3(extrudedPt1);
            insertVec3(pt1Norm);
            insertVec2(uvPt1);
            m_data.push_back(perlinHeightPt1);

            insertVec3(extrudedPt1);
            insertVec3(pt1Norm);
            insertVec2(uvPt1);
            m_data.push_back(perlinHeightPt1);

            insertVec3(extrudedPt4);
            insertVec3(pt4Norm);
            insertVec2(uvPt4);
            m_data.push_back(perlinHeightPt4);

            insertVec3(extrudedPt3);
            insertVec3(pt3Norm);
            insertVec2(uvPt3);
            m_data.push_back(perlinHeightPt3);

            m_minHeight = std::min({m_minHeight, perlinHeightPt1, perlinHeightPt2, perlinHeightPt3, perlinHeightPt4});
            m_maxHeight = std::max({m_maxHeight, perlinHeightPt1, perlinHeightPt2, perlinHeightPt3, perlinHeightPt4});
        }
    }
}

void Terrain::insertExtrudedVertex(int col, int row, glm::vec3 extrudedPt)
{
    // First check if we have a hashmap tied to this latitude.
    if (m_latLongMap.find(col) != m_latLongMap.end()) {
        // If point to insert does not exist, then insert it into our hashmap.
        if (m_latLongMap[col].find(row) == m_latLongMap.at(col).end()) {
            m_latLongMap[col].insert(std::pair<float, glm::vec3>(row, extrudedPt));
        }
    }
    // No hashmap exists for this latitude, so instantiate one and insert it.
    else {
        std::map<float, glm::vec3> toInsert;
        toInsert[row] = extrudedPt;
        m_latLongMap[col] = toInsert;
    }
}

glm::vec3 Terrain::getExistExtrudedNormal(int col, int row)
{
    //    This is a hacky way of fixing the normals at the m_top
    //    float latMax = fmax(FLT_EPSILON, row + m_lat);
    //    float latMin = fmax(FLT_EPSILON, row - m_lat);

    glm::vec3 curr = m_latLongMap[col][row];

    glm::vec3 n0 = curr - m_latLongMap[col][row + 1];
    glm::vec3 n1 = curr - m_latLongMap[col + 1][row + 1];
    glm::vec3 n2 = curr - m_latLongMap[col + 1][row];
    glm::vec3 n3 = curr - m_latLongMap[col + 1][row - 1];
    glm::vec3 n4 = curr - m_latLongMap[col][row - 1];
    glm::vec3 n5 = curr - m_latLongMap[col - 1][row - 1];
    glm::vec3 n6 = curr - m_latLongMap[col - 1][row];
    glm::vec3 n7 = curr - m_latLongMap[col - 1][row - 1];

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

glm::vec2 Terrain::getMinMaxHeight()
{
    return glm::vec2(m_minHeight, m_maxHeight);
}
