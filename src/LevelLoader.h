/**
\file     LevelLoader.h
\brief    Déclaration de la classe LevelLoader.
\author   Blattner Noah
\date     avril 2023
*/

#ifndef WORLDBUILDR_LEVELLOADER_H
#define WORLDBUILDR_LEVELLOADER_H

#include <QString>
#include <QList>
#include "gamecore.h"

class QJsonArray;
class Sprite;

//! A class that can be used to load levels
//!
//! This class is used to easily load levels into the scene of a GameCore.
//! It can load levels from a JSON file.
//!
//! On construction, the class takes a pointer to a GameCore as a parameter.
//! It also takes a QString as a parameter, indicating the path to the folder containing the levels.
//!
//! The class contains a function called loadLevel.
//! This function takes a QString as a parameter.
//! This QString is the name of the level to load.
//! The string must be the name of a JSON file in the folder passed to the constructor.
//! The function returns a QList of the Sprites that were loaded.
//!
//! The class also contains a function called unloadLevel.
//! This function unloads the current level.
//!
//! The class also contains a function called reloadCurrentLevel.
//! This function reloads the current level.
class LevelLoader {
public:
    explicit LevelLoader(GameCore* pCore, const QString& levelsPath);

    QList<Sprite*> loadLevel(const QString& levelName);
    void unloadLevel();
    void reloadCurrentLevel();

private:
    GameCore* m_pCore;
    QString m_levelsPath;
    QString m_currentLevel;

    Sprite* loadSprite(const QJsonObject& spriteObject);
    QList<Sprite*> loadSprites(const QJsonArray& spritesArray);

    void applyParameters(Sprite* pSprite, QString params);
};


#endif //WORLDBUILDR_LEVELLOADER_H
