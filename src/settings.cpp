#include "settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
  Loads the application settings, or, if no saved settings are available,
  loads default values for the settings. You can change the defaults here.
**/
void Settings::loadSettingsOrDefaults()
{
    // Set the default values below
    QSettings s;

    terrainFrequency = s.value("frequency", 0.01f).toFloat();
    terrainPersistance = s.value("persistance", 1.f).toFloat();
    terrainAmplitude = s.value("amplitude", 6.f).toFloat();
    terrainOctaves = s.value("octaves", 4).toInt();

    waterLevel = s.value("waterLevels", 2.95f).toFloat();

    moonNumbers = s.value("moonNumber", 3).toInt();

    useNormalMap = s.value("normalMap", false).toBool();
    useTextureMap = s.value("textureMap", false).toBool();
    useWireframe = s.value("wireframe", false).toBool();
    useWireframe = s.value("freezeTime", false).toBool();


}

void Settings::saveSettings()
{
    QSettings s;

    // Terrain
    s.setValue("frequency", terrainFrequency);
    s.setValue("persistance", terrainPersistance);
    s.setValue("amplitude", terrainAmplitude);
    s.setValue("octaves", terrainOctaves);

    // Water
    s.setValue("waterLevels", waterLevel);

    // Moons
    s.setValue("moonNumber", moonNumbers);


    // Options
    s.value("normalMap", useNormalMap);
    s.value("textureMap", useTextureMap);
    s.value("wireframe", useWireframe);
    s.value("freezeTime", useWireframe);


}
