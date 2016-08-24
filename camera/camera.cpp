#include "camera.h"
#include <iostream>

Camera::Camera(float zoom, float angleX,
               float angleY, int width, int height)
    :
      m_zoom(zoom), m_angleX(angleX), m_angleY(angleY),
      m_width(width), m_height(height)
{
    constructMatrices();
}

void Camera::rebuildMatrices(float zoom, float angleX, float angleY,
                             int width, int height)
{
    m_zoom = zoom;
    m_angleX = angleX;
    m_angleY = angleY;
    m_width = width;
    m_height = height;
    constructMatrices();
}

void Camera::constructMatrices()
{
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));
    m_projection = glm::perspective(0.8f, (float)m_width/m_height, 0.1f, 100.f);
    m_model = glm::mat4(1.f);
}

glm::mat4 Camera::getModelMatrix()
{
    return m_model;
}

glm::mat4 Camera::getViewMatrix()
{
    return m_view;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return m_projection;
}

glm::vec3 Camera::getEye()
{
    glm::mat4 viewModel = glm::inverse(m_model * m_view);
    glm::vec3 cameraPos(viewModel[3]); // Might have to divide by w if you can't assume w == 1
    return cameraPos;
}
