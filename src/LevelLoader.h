/*
 * @file LevelLoader.cpp
 * @brief Définition de la classe LevelLoader.
 * @author Noah Blattner
 * @date Février 2023
 */

#ifndef WORLDBUILDR_LEVELLOADER_H
#define WORLDBUILDR_LEVELLOADER_H

#include <QString>
#include <QList>
#include "gamecore.h"

class QJsonArray;
class Sprite;
class GameScene;

class LevelLoader {
public:
    explicit LevelLoader(GameCore* core, QString levelsPath);

    QList<Sprite*> loadLevel(const QString& levelName);
    void unloadLevel();

private:
    GameCore* m_pCore;
    QString m_levelsPath;

    Sprite* loadSprite(const QJsonObject& spriteObject);
    QList<Sprite*> loadSprites(const QJsonArray& spritesArray);
};


#endif //WORLDBUILDR_LEVELLOADER_H
