#ifndef TEXSPHERE_H
#define TEXSPHERE_H

#include "sphere.h"
#include <QFile>
#include <QImage>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr


class TexSphere
        : public Sphere
{
public:
    TexSphere(int tessFact, float height);

    void init();
private:
    void specifySection(int startRow, int startCol, int endRow, int endCol);

    void calcTangBiTang(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
                        const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2,
                        glm::vec3 &tangent, glm::vec3 &bitangent);
};

#endif // TEXSPHERE_H
