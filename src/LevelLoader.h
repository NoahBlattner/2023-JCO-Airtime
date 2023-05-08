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

    void applyParameters(Sprite* pSprite, const QString params);
};


#endif //WORLDBUILDR_LEVELLOADER_H
