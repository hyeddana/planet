/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    m_viewAngle = glm::radians(60.f);
    m_aspectRatio = 1.f;
    m_near = 1.f;
    m_far = 30.f;
    m_look = glm::vec4(0.f);
    m_eye = glm::vec4(2.f, 2.f, 2.f, 0.f);
    m_up = glm::vec4(0.f, 1.f, 0.f, 0.f);
    m_w = -m_look/glm::length(m_look);
    glm::vec4 temp = (m_up - glm::dot(m_up, m_w)*m_w);
    m_v = temp/glm::length(temp);
    glm::vec3 temp2 = glm::cross(static_cast<glm::vec3>(m_v), static_cast<glm::vec3>(m_w));
    m_u = glm::vec4(temp2.x, temp2.y, temp2.z, 0.f);
}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const
{
    return getPerspectiveMatrix() * getScaleMatrix();
}

glm::mat4x4 CamtransCamera::getViewMatrix() const
{
    glm::mat4x4 m3 =
            glm::transpose(
                glm::mat4x4(
                    m_u.x, m_u.y, m_u.z, m_u.w,
                    m_v.x, m_v.y, m_v.z, m_v.w,
                    m_w.x, m_w.y, m_w.z, m_w.w,
                    0.f, 0.f, 0.f, 1.f
                )
            );
    glm::mat4x4 m4 =
            glm::transpose(
                glm::mat4x4(
                    1.f, 0.f, 0.f, -m_eye.x,
                    0.f, 1.f, 0.f, -m_eye.y,
                    0.f, 0.f, 1.f, -m_eye.z,
                    0.f, 0.f, 0.f, 1.f
                    )
                );

    return m3 * m4;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const
{
    float h2 = tan(m_viewAngle/2.f);
    float height = 2.f * m_far * h2;
    float w2 = height * m_aspectRatio / (2.f * m_far);

    float h = h2 * m_far;
    float w = w2 * m_far;

    return glm::transpose(
                glm::mat4x4(
                    1.f/w, 0.f, 0.f, 0.f,
                    0.f, 1.f/h, 0.f, 0.f,
                    0.f, 0.f, 1.f / m_far, 0.f,
                    0.f, 0.f, 0.f, 1.f
                    )
                );
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const
{
    float c = -1.f*m_near/m_far;
    float x = 1.f/(c + 1.f);
    return glm::transpose(
                glm::mat4x4(1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, -1.f*x, c*x,
                       0.f, 0.f, -1.f, 0.f)
                );
}

glm::vec4 CamtransCamera::getPosition() const
{
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const
{
    return m_look;
}

glm::vec4 CamtransCamera::getUp() const
{
    return m_up;
}

float CamtransCamera::getAspectRatio() const
{
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const
{
    return m_viewAngle;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up)
{
    m_eye = eye;
    m_look = look;
    m_up = up;
    m_w = -1.f*m_look/glm::length(m_look);
    glm::vec4 temp = (m_up - glm::dot(m_up, m_w)*m_w);
    m_v = temp/glm::length(temp);
    glm::vec3 temp2 = glm::cross(static_cast<glm::vec3>(m_v), static_cast<glm::vec3>(m_w));
    m_u = glm::vec4(temp2.x, temp2.y, temp2.z, 0.f);

    m_look = -m_w;
    m_up = m_v;
}

void CamtransCamera::setHeightAngle(float h)
{
    m_viewAngle = glm::radians(h);
}

void CamtransCamera::translate(const glm::vec4 &v)
{
    m_eye += v;
}

void CamtransCamera::rotateU(float degrees)
{
    glm::vec4 u0, v0, w0;
    u0 = m_u;
    v0 = m_v;
    w0 = m_w;
    float rad = glm::radians(degrees);
    m_u = u0;
    m_v = v0*static_cast<float>(cos(rad)) + w0*static_cast<float>(sin(rad));
    m_w = -v0*static_cast<float>(sin(rad)) + w0*static_cast<float>(cos(rad));

    m_look = -m_w;
    m_up = m_v;

}

void CamtransCamera::rotateV(float degrees)
{
    glm::vec4 u0, v0, w0;
    u0 = m_u;
    v0 = m_v;
    w0 = m_w;
    float rad = glm::radians(degrees);
    m_u = u0 * static_cast<float>(cos(rad)) - w0 * static_cast<float>(sin(rad));
    m_v = v0;
    m_w = u0 * static_cast<float>(sin(rad)) + w0 * static_cast<float>(cos(rad));

    m_look = -m_w;
    m_up = m_v;
}

void CamtransCamera::rotateW(float degrees)
{
    glm::vec4 u0, v0, w0;
    u0 = m_u;
    v0 = m_v;
    w0 = m_w;
    float rad = glm::radians(degrees);
    m_u = v0 * static_cast<float>(sin(rad)) + u0 * static_cast<float>(cos(rad));
    m_v = v0 * static_cast<float>(cos(rad)) - u0 * static_cast<float>(sin(rad));
    m_w = w0;

    m_look = -m_w;
    m_up = m_v;
}

void CamtransCamera::setClip(float nearPlane, float farPlane)
{
    m_near = nearPlane;
    m_far = farPlane;
}

