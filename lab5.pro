QT += core gui opengl

TARGET = lab5
TEMPLATE = app

win32 {
    DEFINES += GLEW_STATIC
}

CONFIG += c++11
INCLUDEPATH += src glm cs123_lib glew-1.10.0/include
DEPENDPATH += src glm cs123_lib glew-1.10.0/include

SOURCES += \
    src/mainwindow.cpp \
    src/main.cpp \
    src/glwidget.cpp \
    src/openglshape.cpp \
    cs123_lib/resourceloader.cpp \
    cs123_lib/errorchecker.cpp \
    src/terrain.cpp \
    glew-1.10.0/src/glew.c \
    #Perlin Noise (@gamernb on stackoverflow)
    perlin/PerlinNoise.cpp \
    src/skybox.cpp \
    common/objloader.cpp \
    src/objdrawer.cpp \
    camera/camera.cpp \
    src/sphere.cpp \
    src/texsphere.cpp \
    src/settings.cpp \
    src/databinding.cpp \
    src/loadingscreen.cpp

HEADERS += \
    src/mainwindow.h \
    src/glwidget.h \
    src/openglshape.h \
    cs123_lib/resourceloader.h \
    cs123_lib/errorchecker.h \
    src/terrain.h \
    glew-1.10.0/include/GL/glew.h \
    #Perlin Noise (@gamernb on stackoverflow)
    perlin/PerlinNoise.h \
    src/skybox.h \
    common/objloader.h \
    src/objdrawer.h \
    src/sphere.h \
    camera/camera.h \
    src/texsphere.h \
    src/settings.h \
    src/databinding.h \
    src/loadingscreen.h


FORMS += src/mainwindow.ui

OTHER_FILES += \
    shaders/shader.frag \
    shaders/shader.vert \
    shaders/skybox.frag \
    shaders/skybox.vert \
    shaders/lighting.frag \
    shaders/lighting.vert \
    shaders/loading.frag \
    shaders/loading.vert


RESOURCES += \
    shaders/shaders.qrc \
    images/images.qrc

