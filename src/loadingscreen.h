#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
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
#include "openglshape.h"
#include <memory>

class LoadingScreen
{
public:
    LoadingScreen();
    void draw();

private:
    std::unique_ptr<OpenGLShape> m_image;
    GLuint VBO, VAO;
    void init();
};

#endif // LOADINGSCREEN_H
