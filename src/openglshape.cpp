#include "openglshape.h"

void OpenGLShape::create()
{
    // Generates a VBO and VAO for this shape.
    glGenBuffers(1, &m_vboID);
    glGenVertexArrays(1, &m_vaoID);
}

void OpenGLShape::destroy()
{
    // Deletes the VBO and VAO.
    glDeleteBuffers(1, &m_vboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

void OpenGLShape::setVertexData(float *data, GLsizeiptr size, GLenum drawMode, int numVertices)
{
    m_drawMode = drawMode;
    m_numVertices = numVertices;

    // Stores vertex data in VBO.
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLShape::setAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t pointer)
{
    // Bind VAO and VBO.
    glBindVertexArray(m_vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

    // Defines the specified vertex attribute with respect to the VBO's data store.
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, (void*) pointer);

    // Unbind VAO and VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OpenGLShape::draw()
{
    glBindVertexArray(m_vaoID);
    glDrawArrays(m_drawMode, 0, m_numVertices);
    glBindVertexArray(0);
}
