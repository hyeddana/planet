#include "texsphere.h"

#include <iostream>
TexSphere::TexSphere(int tessFact, float height) :
    Sphere(tessFact, height)
{
    std::cout << "[OCEAN] building ocean..." << std::endl;
    init();

}

void TexSphere::init() {
    // TODO: Change from GL_LINE to GL_FILL in order to render full triangles instead of wireframe.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::vec2 uvTop = findUV(m_top);
    glm::vec2 uvBottom = findUV(m_top);

    // Calculate points for caps.
    for (int i = 0; i < m_numRows; i++) {
        glm::vec3 point1 = getPosition(m_lat,  m_long * i);
        glm::vec3 point2 = getPosition(m_lat,  (m_long * i) + m_long);

        glm::vec2 uv1 = findUV(point1);
        glm::vec2 uv2 = findUV(point2);

        glm::vec3 tangent;
        glm::vec3 bitangent;
        calcTangBiTang(m_top, point1, point2, uvTop, uv1, uv2, tangent, bitangent);

        // v0
        insertVec3(point2);
        insertVec3(glm::normalize(point2));
        insertVec2(uv2);
        insertVec3(tangent);
        insertVec3(bitangent);

        // v1
        insertVec3(point1);
        insertVec3(glm::normalize(point1));
        insertVec2(uv1);
        insertVec3(tangent);
        insertVec3(bitangent);

        // v2
        insertVec3(m_top);
        insertVec3(glm::normalize(m_top));
        insertVec2(uvTop);
        insertVec3(tangent);
        insertVec3(bitangent);

    }
    for (int i = 0; i < m_numRows; i++) {
        glm::vec3 point1 = getPosition(m_lat * (m_numCols - 1),  m_long * i);
        glm::vec3 point2 = getPosition(m_lat * (m_numCols - 1),  (m_long * i) + m_long);

        glm::vec2 uv1 = findUV(point1);
        glm::vec2 uv2 = findUV(point2);

        glm::vec3 tangent;
        glm::vec3 bitangent;
        calcTangBiTang(point1, m_bottom, point2, uv1, uvBottom, uv2, tangent, bitangent);

        // v0
        insertVec3(m_bottom);
        insertVec3(glm::normalize(m_bottom));
        insertVec2(uvBottom);
        insertVec3(tangent);
        insertVec3(bitangent);

        // v1
        insertVec3(point1);
        insertVec3(glm::normalize(point1));
        insertVec2(uv1);
        insertVec3(tangent);
        insertVec3(bitangent);

        // v2
        insertVec3(point2);
        insertVec3(glm::normalize(point2));
        insertVec2(uv2);
        insertVec3(tangent);
        insertVec3(bitangent);
    }

    specifySection(0, 1, m_numRows, m_numCols - 1);
//    specifySection(m_numRows - 1, 1, m_numRows, m_numCols - 1);

    int numVertices = m_data.size();

    // Initialize OpenGLShape.
    m_shape.create();
    m_shape.setVertexData(&m_data[0], sizeof(GLfloat) * numVertices, GL_TRIANGLES, numVertices);
    m_shape.setAttribute(0, 3, GL_FLOAT, GL_FALSE, (3*4 + 2) * sizeof(GLfloat), 0);
    m_shape.setAttribute(1, 3, GL_FLOAT, GL_FALSE, (3*4 + 2) * sizeof(GLfloat), 3 * sizeof(GLfloat));
    m_shape.setAttribute(2, 2, GL_FLOAT, GL_FALSE, (3*4 + 2) * sizeof(GLfloat), 6 * sizeof(GLfloat));
    m_shape.setAttribute(3, 3, GL_FLOAT, GL_FALSE, (3*4 + 2) * sizeof(GLfloat), 8 * sizeof(GLfloat));
    m_shape.setAttribute(4, 3, GL_FLOAT, GL_FALSE, (3*4 + 2) * sizeof(GLfloat), 11 * sizeof(GLfloat));
}

void TexSphere::specifySection(int startRow, int startCol, int endRow, int endCol)
{
    for (int row = startRow; row < endRow; row++) {
        for (int col = startCol; col < endCol; col++) {
            glm::vec3 pt1 = getPosition(m_lat * col, m_long * row);
            glm::vec3 pt2 = getPosition((m_lat * col) + m_lat, m_long * row);
            glm::vec3 pt3 = getPosition((m_lat * col) + m_lat, (m_long * row) + m_long);
            glm::vec3 pt4 = getPosition((m_lat * col), (m_long * row) + m_long);

            if (row == endRow - 1) {
                pt3 = getPosition((m_lat * col) + m_lat, (m_long * row) + m_long - 0.000001);
                pt4 = getPosition((m_lat * col), (m_long * row) + m_long - 0.000001);
            }

            glm::vec3 pt1Norm = glm::normalize(pt1);
            glm::vec3 pt2Norm = glm::normalize(pt2);
            glm::vec3 pt3Norm = glm::normalize(pt3);
            glm::vec3 pt4Norm = glm::normalize(pt4);

            glm::vec2 uv1 = findUV(pt1);
            glm::vec2 uv2 = findUV(pt2);
            glm::vec2 uv3 = findUV(pt3);
            glm::vec2 uv4 = findUV(pt4);



            glm::vec3 tangent;
            glm::vec3 bitangent;
            calcTangBiTang(pt1, pt2, pt3, findUV(pt1), findUV(pt2), findUV(pt3), tangent, bitangent);

            insertVec3(pt3);
            insertVec3(pt3Norm);
            insertVec2(uv3);
            insertVec3(tangent);
            insertVec3(bitangent);

            insertVec3(pt2);
            insertVec3(pt2Norm);
            insertVec2(uv2);
            insertVec3(tangent);
            insertVec3(bitangent);

            insertVec3(pt1);
            insertVec3(pt1Norm);
            insertVec2(uv1);
            insertVec3(tangent);
            insertVec3(bitangent);

            insertVec3(pt1);
            insertVec3(pt1Norm);
            insertVec2(uv1);
            insertVec3(tangent);
            insertVec3(bitangent);

            insertVec3(pt4);
            insertVec3(pt4Norm);
            insertVec2(uv4);
            insertVec3(tangent);
            insertVec3(bitangent);

            insertVec3(pt3);
            insertVec3(pt3Norm);
            insertVec2(uv3);
            insertVec3(tangent);
            insertVec3(bitangent);
        }
    }
}


void TexSphere::calcTangBiTang(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
                               const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2,
                               glm::vec3 &tangent, glm::vec3 &bitangent)
{
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec2 deltaUV1 = uv1 - uv0;
    glm::vec2 deltaUV2 = uv2 - uv0;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = glm::normalize(tangent);

    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent = glm::normalize(bitangent);
}
