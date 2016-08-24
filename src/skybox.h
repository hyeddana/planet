#ifndef SKYBOX_H
#define SKYBOX_H
#include "GL/glew.h"
#include <QGLWidget>
#include <QTimer>
#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif

#include <QFile>
#include <iostream>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr


using namespace std;

class Skybox
{
public:
    Skybox();
    void draw();

    GLuint m_textureID;
    GLuint getTextureID();

private:

    GLuint m_skyboxVAO, m_skyboxVBO;

    bool loadCubemap(std::string name, GLuint textureID, GLenum side);

    GLuint initCubeMap();

};

#endif // SKYBOX_H
