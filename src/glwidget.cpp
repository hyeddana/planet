#include "glwidget.h"
#include "settings.h"

#include "cs123_lib/resourceloader.h"
#include "cs123_lib/errorchecker.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include <QLabel>
#include <skybox.h>
#include <memory>
#include "glm/ext.hpp"
#include <stdlib.h>

#include <common/objloader.h>
#include <src/objdrawer.h>

#define PI 3.14159265f

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent), m_angleX(0), m_angleY(0.5f), m_zoom(10.f),
      m_fps(60.0f),
      m_increment(0)
{
    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));

    timer.start(1000.0f / m_fps);
}

GLWidget::~GLWidget()
{ }

void GLWidget::initializeGL()
{
    m_camera.reset(new Camera(m_zoom, m_angleX, m_angleY, width(), height()));

    ResourceLoader::initializeGlew();
    resizeGL(width(), height());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    m_program = ResourceLoader::createShaderProgram(":/shaders/shader.vert", ":/shaders/shader.frag");
    m_skyboxprogram = ResourceLoader::createShaderProgram(":/shaders/skybox.vert", ":/shaders/skybox.frag");
    m_lightingprogram = ResourceLoader::createShaderProgram(":/shaders/lighting.vert", ":/shaders/lighting.frag");
    m_loadingprogram = ResourceLoader::createShaderProgram(":/shaders/loading.vert", ":/shaders/loading.frag");


    /* initialize light */
    m_light = glm::vec3(-100.f, 0.f, -100.f);

    m_lightOcean = glm::vec3(0.f, 0.f, 10.f);


//    generate();
    initializeObjects();


    rebuildMatrices();
}

void GLWidget::initializeObjects()
{

    // Remove previous data
    m_texSphereData.clear();
    m_texSpheres.clear();

    // PROCEDURAL MODIFICATION VARIABLES
    int texsphereTess = 100;
    float oceanHeight = settings.waterLevel;
    int numMoons = glm::clamp(settings.moonNumbers, 0, 10);
    // END

    glm::vec3 diffuse_color, ambient_color, specular_color;
    float shininess;
    bool isMoon, useNormalMap, useDiffuseMap;

    /* create loading screen */
    m_loading.reset(new LoadingScreen());

    /* create terrain */
    m_terrain.reset(new Terrain(settings.terrainFrequency, settings.terrainPersistance,
                                settings.terrainAmplitude, settings.terrainOctaves));

    unique_ptr<TexSphere> ts;

    /* create ocean */
    /* ocean color */
    diffuse_color = glm::vec3(0, 0, 0.2f);
    ambient_color = glm::vec3(0.1f);
    specular_color = glm::vec3(0.2f);
    shininess = 2.f;
    isMoon = false;
    useNormalMap = true;
    useDiffuseMap = false;
    ts.reset(new TexSphere(texsphereTess, oceanHeight));
    m_texSpheres.push_back(*ts);
    m_texSphereData.push_back(TexSphereData(diffuse_color, ambient_color, specular_color, shininess,
                                            isMoon,
                                            useNormalMap, useDiffuseMap));

    /* create moon(s) */
    for (int i = numMoons; i > 0; i--)
    {
        /* since the moon is diffuse mapped the ambient, specular, etc
         * vecs don't matter */
        isMoon = true;
        useNormalMap = false;
        useDiffuseMap = true;
        float offsetFromCenter = 10.f + static_cast<float>(i)*5.f;
        float moonHeight = (random() % 3) * ((random() % 2) * -1.f);
        float moonSpeed = 3.f;
        float moonRadius = ((rand() % 5) + 1) / 10.f;

        ts.reset(new TexSphere(texsphereTess, moonRadius));
        m_texSpheres.push_back(*ts);
        m_texSphereData.push_back(TexSphereData(diffuse_color, ambient_color, specular_color, shininess,
                                                isMoon,
                                                offsetFromCenter, moonHeight, moonSpeed,
                                                useNormalMap, useDiffuseMap));
    }

    /* instantiate skybox */
    m_skybox.reset(new Skybox());


    /* load textures */
    m_oceanNormalMap = loadImage(":/images/ocean.jpg");
    m_moonTexture = loadImage(":/images/moon_texture.jpg");
    m_planetCreviceTex = loadImage(":/images/deep_crevice.jpg");
    m_planetSandTex = loadImage(":/images/sand.jpg");
    m_planetGrassTex = loadImage(":/images/grass.jpg");
    m_planetConcreteTex = loadImage(":/images/concrete.jpg");
    m_planetSnowTex = loadImage(":/images/snow.jpg");
    m_skyboxTex = m_skybox->getTextureID();
    m_loadingScreenTex = loadImage(":/images/moon_texture.jpg");
}

void GLWidget::paintGL()
{
    int mode = GL_FILL;

    if (settings.useWireframe) {
        mode = GL_LINE;
    }


    glPolygonMode(GL_FRONT_AND_BACK, mode);

    // Set the color to set the screen when the color buffer is cleared.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float time;
    if (!settings.freezeTime) {
        time = m_increment++ / (float) m_fps;
    } else  {
        time = m_increment / (float) m_fps;
    }


//    glUseProgram(m_loadingprogram);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_loadingScreenTex);
//    glUniform1i(glGetUniformLocation(m_program, "sampler"), 0);
//    m_loading->draw();
//    glUseProgram(0);


    glm::vec3 eye = m_camera->getEye();

    /* Planet Generation */
    glDepthMask(GL_TRUE);
    // Bind shader program.
    glUseProgram(m_program);
    // Set uniforms.
    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniform3fv(glGetUniformLocation(m_program, "viewPos"), 1, glm::value_ptr(eye));
    glUniform3fv(glGetUniformLocation(m_program, "light"), 1, glm::value_ptr(m_light));
    glUniform1i(glGetUniformLocation(m_program, "useTerrainText"), (settings.useTextureMap));

    glm::vec2 minMaxHeight = m_terrain->getMinMaxHeight();
    float heightSplit = (minMaxHeight.y - minMaxHeight.x) / 5.f;

    glUniform1f(glGetUniformLocation(m_program, "minHeight"), minMaxHeight.x);
    glUniform1f(glGetUniformLocation(m_program, "maxHeight"), minMaxHeight.y);
    glUniform1f(glGetUniformLocation(m_program, "heightSplit"), heightSplit);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_planetCreviceTex);
    glUniform1i(glGetUniformLocation(m_program, "deepCrevice"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_planetSandTex);
    glUniform1i(glGetUniformLocation(m_program, "sand"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_planetGrassTex);
    glUniform1i(glGetUniformLocation(m_program, "grass"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_planetConcreteTex);
    glUniform1i(glGetUniformLocation(m_program, "concrete"), 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_planetSnowTex);
    glUniform1i(glGetUniformLocation(m_program, "snow"), 4);
    // Draw terrain.
    m_terrain->draw();
    // Unbind shader program.
    glUseProgram(0);


    /* main shading pipeline loop */
    glUseProgram(m_lightingprogram);

    // Set uniforms.
    glUniformMatrix4fv(glGetUniformLocation(m_lightingprogram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_lightingprogram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniform3fv(glGetUniformLocation(m_lightingprogram, "viewPos"), 1, glm::value_ptr(eye));
    glUniform3fv(glGetUniformLocation(m_lightingprogram, "light"), 1, glm::value_ptr(m_light));

    for (int i = 0; i < m_texSpheres.size(); i++)
    {
        glUniform3fv(glGetUniformLocation(m_lightingprogram, "light"), 1, glm::value_ptr(m_light));

        TexSphereData data = m_texSphereData.at(i);
        glm::mat4 translateMat;
        if (data.isMoon) {
            /* Calculate translation and rotation of moon. */
            float angle = (float) PI/2 * time / (float)(i + 1);

            float rotationX = (sin(angle) * data.offsetFromCenter) / data.moonSpeed;
            float rotationY = data.moonHeight;
            float rotationZ = (cos(angle) * data.offsetFromCenter) / data.moonSpeed;

            translateMat = glm::translate(glm::vec3(rotationX, rotationY, rotationZ));
        } else {
            /* it must be ocean */
            float factor = 10.f;
            float degRot = fabs(sin((PI / 2) * time)) / factor;

            translateMat = glm::rotate(m_model, degRot, glm::vec3(1, 1, 0));
            if (data.useNormalMap && settings.useNormalMap) {

                glUniform3fv(glGetUniformLocation(m_lightingprogram, "light"), 1, glm::value_ptr(m_lightOcean));
            }
        }

        glUniformMatrix4fv(glGetUniformLocation(m_lightingprogram, "model"), 1, GL_FALSE, glm::value_ptr(translateMat));

        glUniform3fv(glGetUniformLocation(m_lightingprogram, "diffuse_color"), 1, glm::value_ptr(data.diffuse_color));
        glUniform3fv(glGetUniformLocation(m_lightingprogram, "ambient_color"), 1, glm::value_ptr(data.ambient_color));
        glUniform3fv(glGetUniformLocation(m_lightingprogram, "specular_color"), 1, glm::value_ptr(data.specular_color));
        glUniform1f(glGetUniformLocation(m_lightingprogram, "shininess"), data.shininess);
        glUniform1i(glGetUniformLocation(m_lightingprogram, "useNormalMap"), (data.useNormalMap && settings.useNormalMap));
        glUniform1i(glGetUniformLocation(m_lightingprogram, "useDiffuseMap"), (data.useDiffuseMap && settings.useTextureMap));

        if ((data.useDiffuseMap && settings.useTextureMap) || (data.useNormalMap && settings.useNormalMap))
        {
            glActiveTexture(GL_TEXTURE0);

            if ((data.useNormalMap && settings.useNormalMap)) {
                glBindTexture(GL_TEXTURE_2D, m_oceanNormalMap);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, m_moonTexture);
            }
        }
        m_texSpheres.at(i).draw();
    }
    // unbind texture program
    glUseProgram(0);


    /*                         */
    /* skybox and model loader */
    /*                         */

    /** Skybox Generation **/
    glDepthMask(GL_FALSE);
    glUseProgram(m_skyboxprogram);

    // Calculate un-translated view matrix (so that the skybox
    // does not move with the camera)

    glm::mat4 finalv = glm::mat4(glm::mat3(m_view));

    // Set uniforms.
    glUniformMatrix4fv(glGetUniformLocation(m_skyboxprogram, "view"), 1, GL_FALSE, glm::value_ptr(finalv));
    glUniformMatrix4fv(glGetUniformLocation(m_skyboxprogram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTex);
    //  Draw skybox.
    m_skybox->draw();
    //  Unbind shader program.

    glUseProgram(0);
    glDepthMask(GL_TRUE);

    unique_ptr<objDrawer> obj;
    obj.reset(new objDrawer(m_program));
//    obj->draw("/home/hyeddana/course/planetofthegreats/tree.obj");
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    rebuildMatrices();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_angleX += 10 * (event->x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    m_zoom -= event->delta() / 100.f;
    rebuildMatrices();
}

void GLWidget::rebuildMatrices()
{
    m_camera->rebuildMatrices(m_zoom, m_angleX, m_angleY, width(), height());
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));
    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
    m_model = glm::mat4(1.f);
    update();
}

void GLWidget::drawLoading() {

    printf("calling loading");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glUseProgram(m_loadingprogram);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_loadingScreenTex);
//    glUniform1i(glGetUniformLocation(m_program, "sampler"), 0);
//    m_loading->draw();
//    glUseProgram(0);
}


void GLWidget::generate() {

    drawLoading();
    initializeObjects();
}

void GLWidget::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001f;

    update();
}


GLuint GLWidget::loadImage(std::string path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;

    // Load the image.
    QString filename = QString::fromStdString(path);
    QFile file(filename);
    QImage image;
    image.load(file.fileName());


    width = image.width();
    height = image.height();

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    //  Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
