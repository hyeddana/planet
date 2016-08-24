#include "loadingscreen.h"

LoadingScreen::LoadingScreen()
{
    init();
}

void LoadingScreen::init()
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // points               // uv
//        -1.0f, -1.0f, 0.0f, 0.0, 0.0, // Left
//         1.0f, -1.0f, 0.0f, 0.0, 1.0, // Right
//        -1.0f,  1.0f, 0.0f, 1.0, 0.0, // Top
//        -1.0f,  1.0f, 0.0f, 1.0, 0.0, // Left
//        1.0f, -1.0f, 0.0f, 0.0, 1.0, // Right
//        1.0f, 1.0f, 0.0f, 1.0, 1.0// Top

        -0.5f, 0.5f, 0.0f,
         0.0f, 0.0f,

         -0.5f, -0.5f, 0.0f,
         0.0f, 1.0f,

         0.0f, 0.5f, 0.0f,
         0.5f, 0.0f,

         0.0f, -0.5f, 0.0f,
         0.5f, 1.0f,

         0.5f, 0.5f, 0.0f,
         1.0f, 0.0f,

         0.5f, -0.5f, 0.0f,
         1.0f, 1.0f,
    };

    // Setup loading VAO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), &vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)12.f);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LoadingScreen::draw()
{
    // Draw our first triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
