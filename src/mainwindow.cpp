

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databinding.h"
#include "settings.h"


#include <QSettings>
#include <assert.h>
#include <QGridLayout>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    // Make sure the settings are loaded before the UI
    settings.loadSettingsOrDefaults();


    this->setMinimumSize(100, 100);
    QGLFormat qglFormat;
    qglFormat.setVersion(4,0);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    m_ui->setupUi(this);
    QGridLayout *gridLayout = new QGridLayout(m_ui->centralWidget);
    m_glWidget = new GLWidget(qglFormat, this);
    gridLayout->addWidget(m_glWidget, 0, 1);



    QSettings qtSettings("Planets");
//    restoreGeometry(qtSettings.value("geometry").toByteArray());
    restoreState(qtSettings.value("windowState").toByteArray());

    dataBind();

    connect(m_ui->generate, SIGNAL (released()), this, SLOT (generate()));

}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings)
        delete b;

    delete m_ui;
    delete m_glWidget;
}
void MainWindow::dataBind()
{

#define BIND(b) { \
    DataBinding *_b = (b); \
    m_bindings.push_back(_b); \
}
    // Terrain
    BIND(FloatBinding::bindSliderAndTextbox(
        m_ui->frequency, m_ui->freqDisplay, settings.terrainFrequency, 0.f, 1.5f))

    BIND(FloatBinding::bindSliderAndTextbox(
        m_ui->persistance, m_ui->persistanceDisplay, settings.terrainPersistance, 0.f, 1.f))

    BIND(FloatBinding::bindSliderAndTextbox(
        m_ui->amplitude, m_ui->amplitudeDisplay, settings.terrainAmplitude, 0.f, 10.f))

    BIND(IntBinding::bindSliderAndTextbox(
        m_ui->octaves, m_ui->octavesDisplay, settings.terrainOctaves, 0, 6))

    // Water
    BIND(FloatBinding::bindSliderAndTextbox(
        m_ui->waterLevels, m_ui->waterDisplay, settings.waterLevel, 2.f, 5.f))

    // Moons
    BIND(IntBinding::bindSliderAndTextbox(
        m_ui->moonNumber, m_ui->moonDisplay, settings.moonNumbers, 0, 6))

    // Options
    BIND(BoolBinding::bindCheckbox(m_ui->textureMap, settings.useTextureMap))
    BIND(BoolBinding::bindCheckbox(m_ui->normalMap, settings.useNormalMap))
    BIND(BoolBinding::bindCheckbox(m_ui->wireframe, settings.useWireframe))
    BIND(BoolBinding::bindCheckbox(m_ui->freezeTime, settings.freezeTime))


#undef BIND

}

void MainWindow::generate() {
    m_glWidget->generate();
}

void MainWindow::closeEvent(QCloseEvent *event) {

    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("Planets");
//    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);

}

