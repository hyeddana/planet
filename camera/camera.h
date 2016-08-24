#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera
{
public:
    Camera(float zoom, float angleX, float angleY, int width, int height);

    void rebuildMatrices(float zoom, float angleX, float angleY, int width, int height);

    glm::mat4 getModelMatrix();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

    glm::vec3 getEye();


private:
    float m_zoom, m_angleX, m_angleY;

    int m_width, m_height;

    glm::mat4 m_model, m_view, m_projection;

    void constructMatrices();
};

#endif // CAMERA_H
