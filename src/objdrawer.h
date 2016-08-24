#ifndef OBJDRAWER_H
#define OBJDRAWER_H
#include "GL/glew.h"
#include <QGLWidget>
#include <QTimer>
#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif

using namespace std;

#include <string>

class objDrawer
{
public:
    objDrawer(GLuint program);
    void draw(std::string filepath);

private:
    GLuint m_program;
};

#endif // OBJDRAWER_H
