#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "GL/glew.h"
#include <QGLWidget>
#include <QTime>
#include <QTimer>
#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif
#include "terrain.h"
#include <camera/camera.h>
#include "texsphere.h"
#include <camera/CamtransCamera.h>
#include <loadingscreen.h>

class Camera;

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "skybox.h"
#include <memory>
#include <QTime>
#include <QTimer>

struct TexSphereData
{
    glm::vec3 diffuse_color, ambient_color, specular_color;
    float shininess;
    bool isMoon;
    float offsetFromCenter, moonHeight, moonSpeed; // for moons
    bool useNormalMap, useDiffuseMap;

    /* constructor for moon objects */
    TexSphereData(glm::vec3 diffuse_color, glm::vec3 ambient_color,
                  glm::vec3 specular_color,
                  float shininess, bool isMoon,
                  float offsetFromCenter, float moonHeight, float moonSpeed,
                  bool useNormalMap, bool useDiffuseMap) :
        diffuse_color(diffuse_color), ambient_color(ambient_color),
        specular_color(specular_color), shininess(shininess), isMoon(isMoon),
        offsetFromCenter(offsetFromCenter), moonHeight(moonHeight), moonSpeed(moonSpeed),
        useNormalMap(useNormalMap),
        useDiffuseMap(useDiffuseMap)
    {}

    /* constructor for generic tex sphere objects */
    TexSphereData(glm::vec3 diffuse_color, glm::vec3 ambient_color,
                  glm::vec3 specular_color,
                  float shininess, bool isMoon,
                  bool useNormalMap, bool useDiffuseMap) :
        diffuse_color(diffuse_color), ambient_color(ambient_color),
        specular_color(specular_color), shininess(shininess), isMoon(isMoon),
        offsetFromCenter(0.f), moonHeight(0.f), moonSpeed(0.f),
        useNormalMap(useNormalMap),
        useDiffuseMap(useDiffuseMap)
    {}
};

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QGLFormat format, QWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void initializeObjects();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

public slots:
    void generate();

private slots:
    void tick();

private:
    void rebuildMatrices();

    void drawLoading();

    GLuint loadImage(std::string path);

    /** ID for the shader program. */
    GLuint m_program, m_skyboxprogram, m_lightingprogram, m_loadingprogram;

    unique_ptr<Terrain> m_terrain;
    std::vector<TexSphere> m_texSpheres;
    std::vector<TexSphereData> m_texSphereData;

    glm::vec3 m_light, m_lightOcean;

    glm::mat4 m_model, m_view, m_projection;

    unique_ptr<Camera> m_camera;

    /** For mouse interaction. */
    float m_angleX, m_angleY, m_zoom;
    QPoint m_prevMousePos;

    /** Skybox stuff */
    std::unique_ptr<Skybox> m_skybox;

    /** Loading Screen **/
    std::unique_ptr<LoadingScreen> m_loading;

    /** Texture IDs */
    GLuint m_oceanNormalMap, m_moonTexture,
           m_planetCreviceTex, m_planetSandTex, m_planetGrassTex, m_planetConcreteTex,
           m_planetSnowTex, m_skyboxTex, m_loadingScreenTex;

    /** Timer stuff */
    QTime time;
    QTimer timer;
    float m_fps;

    /** Incremented on every call to paintGL. */
    int m_increment;
};

#endif // GLWIDGET_H
