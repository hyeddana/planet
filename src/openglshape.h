#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

#include "GL/glew.h"
#include <vector>

class OpenGLShape
{
public:
    OpenGLShape() { }

    virtual ~OpenGLShape() { }

    /** Creates a new VBO and VAO on the GPU. */
    void create();

    /** Deletes the VBO and VAO from the GPU. */
    void destroy();

    /** Initialize the VBO with the given vertex data. */
    void setVertexData(float *data, GLsizeiptr size, GLenum drawMode, int numVertices);

    /** Enables the specified attribute and calls glVertexAttribPointer with the given arguments. */
    void setAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized,
                      GLsizei stride, size_t pointer);

    /** Draw the initialized geometry. */
    void draw();

private:
    GLuint m_vboID;    /// ID of the vertex buffer object (VBO)
    GLuint m_vaoID;    /// ID of the vertex array object (VAO)
    int m_numVertices; /// Number of vertices to be drawn.
    GLenum m_drawMode; /// Drawing mode for glDrawArrays (e.g. GL_TRIANGLES, etc)
};

#endif // OPENGLSHAPE_H
