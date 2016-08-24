#include "skybox.h"
#include "resourceloader.h"

Skybox::Skybox()
{
    std::cout << "skybox: connect to shader" << std::endl;

    // create cube mpa
    initCubeMap();
    std::cout << "finished loading cube images" << endl;

    // load buffer
    GLfloat skyboxVertices[] = {
        // Positions
        -50.0f,  50.0f, -50.0f,
        -50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,

        -50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,

         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,

        -50.0f, -50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,

        -50.0f,  50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f, -50.0f,

        -50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f,  50.0f
    };

    // Setup skybox VAO
    glGenBuffers(1, &m_skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_skyboxVAO);
    glBindVertexArray(m_skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::draw()
{
    glBindVertexArray(m_skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

GLuint Skybox::initCubeMap()
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_textureID);

    loadCubemap(":/images/back.png", m_textureID, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    loadCubemap(":/images/front.png", m_textureID, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    loadCubemap(":/images/back.png", m_textureID, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    loadCubemap(":/images/back.png", m_textureID, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    loadCubemap(":/images/top.png", m_textureID, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    loadCubemap(":/images/top.png", m_textureID, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

bool Skybox::loadCubemap(std::string name, GLuint textureID, GLenum side)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    QString filename = QString::fromStdString(name);
    QFile file(filename);
    QImage image;
    image.load(file.fileName());

    QImage textureToUse = QGLWidget::convertToGLFormat(image);

    glTexImage2D(side, 0, GL_RGBA, textureToUse.width(),
                 textureToUse.height(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, textureToUse.bits());

}

GLuint Skybox::getTextureID()
{
    return m_textureID;
}
