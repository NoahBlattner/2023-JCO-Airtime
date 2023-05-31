
//! @file LevelLoader.cpp
//! @brief Définition de la classe LevelLoader.
//! @author Noah Blattner
//! @date Février 2023

#include <QDir>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "LevelLoader.h"
#include "gamescene.h"
#include "sprite.h"
#include "resources.h"
#include "Player.h"
#include "DirectionalEntityCollider.h"
#include "AdvancedCollisionSprite.h"
#include "LevelTrigger.h"
#include "DashRefill.h"

//! Constructor :
//! \param core The game core managing the scene in which the level will be loaded.
//! \param levelsPath Le chemin des niveaux.
LevelLoader::LevelLoader(GameCore* pCore, const QString& levelsPath) {
    m_pCore = pCore;
    m_levelsPath = levelsPath;
}

//! Loads a level in the scene.
//! \param levelName The name of the level to load.
//! \return The list of sprites loaded.
QList<Sprite *> LevelLoader::loadLevel(const QString& levelName) {
    // Concaténation du chemin du niveau avec le nom du niveau
    QString levelPath = m_levelsPath + "/" + levelName;
    qDebug() << "Chargement du niveau " << levelPath;

    if (!levelName.endsWith(".json")) { // If the level name doesn't end with ".json"
        levelPath += ".json";
    }

    levelPath = QDir::toNativeSeparators(levelPath);

    if (!QFile::exists(levelPath)) { // SIf the file doesn't exist
        // On affiche une erreur
        QMessageBox::critical(nullptr, "Erreur", "Le fichier de niveau " + levelName + " n'existe pas.");
        return {};
    }

    QFile file(levelPath);
    if (!file.open(QIODevice::ReadOnly)) { // If the file can't be opened
        // On affiche une erreur
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier de niveau " + levelName + ".");
        return {};
    }

    unloadLevel(); // Unload the current level

    // Load the data from the JSON file
    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject levelObject = jsonDocument.object();

    // Remember the current level's name
    m_currentLevel = levelName;

    // Adapt scene size
    int sceneWidth = levelObject["sceneWidth"].toInt();
    int sceneHeight = levelObject["sceneHeight"].toInt();
    m_pCore->scene()->setSceneRect(0, 0, sceneWidth, sceneHeight);

    // Load the background image
    QImage backgroundImage = QImage(GameFramework::imagesPath() + levelObject["background"].toString());
    backgroundImage = backgroundImage.scaled(sceneWidth, sceneHeight);
    m_pCore->scene()->setBackgroundImage(backgroundImage);

    // Load the sprites
    return loadSprites(levelObject["sprites"].toArray());
}

//! Loads sprites into the scene.
//! \param spritesArray The JSON array containing the sprites data.
QList<Sprite*> LevelLoader::loadSprites(const QJsonArray& spritesArray) {
    QList<Sprite*> sprites;

    // Pour chaque sprite
    for (QJsonValue spriteValue : spritesArray) {
        // On charge la sprite
        Sprite* sprite = loadSprite(spriteValue.toObject());
        sprites.append(sprite);
    }

    return sprites;
}

//! Load a sprite from a JSON object.
//! Loads a subclass of Sprite if specified in the tag.
//! \param spriteObject The JSON object containing the sprite data.
//! \return The loaded sprite.
Sprite* LevelLoader::loadSprite(const QJsonObject &spriteObject) {
    // Get the tag
    QList<QString> tagInfos = spriteObject["tag"].toString().split("?");
    QString tag = tagInfos[0];

    QString imagePath = QDir::toNativeSeparators(GameFramework::imagesPath() +
            spriteObject["textureName"].toString());

    Sprite* sprite = generateSprite(tag, imagePath);

    // Apply transformations
    sprite->setTransformOriginPoint(sprite->globalBoundingRect().center());
    sprite->setPos(spriteObject["x"].toDouble(), spriteObject["y"].toDouble());
    sprite->setScale(spriteObject["scale"].toDouble());
    sprite->setRotation(spriteObject["rotation"].toInt());
    sprite->setZValue(spriteObject["z-index"].toInt());
    sprite->setOpacity(spriteObject["opacity"].toDouble());

    // If additional parameters are specified, apply them
    if (tagInfos.size() > 1) {
        applyParameters(sprite, tagInfos[1]);
    }

    m_pCore->scene()->addSpriteToScene(sprite); // Add the sprite to the scene

    return sprite;
}

//! Generates a sprite from a tag.
//! Creates a new sprite or a subclass of Sprite depending on the tag.
//! \param tag The tag of the sprite.
//! \param imagePath The path of the image of the sprite.
//! \return The generated sprite.
Sprite* LevelLoader::generateSprite(const QString &tag, const QString &imagePath) const {
    Sprite* sprite;

    // If the tag is empty
    if (tag.isEmpty()) {
        // Create a simple sprite
        sprite = new Sprite(imagePath);
    }

    // If the tag is a LevelTrigger
    else if (tag.startsWith("LevelTrigger")) {
        // Create a level trigger
        sprite = new LevelTrigger(m_pCore, tag . split("-")[1]);
    }

    // If the tag is a DirectionalCollider
    else if (tag.startsWith("DirectionalCollider")) {
        DirectionalEntityCollider::BlockingSides blockingSides;

        // Define the blocking sides
        if (tag.contains("Top"))
            blockingSides.top = true;
        if (tag.contains("Bottom"))
            blockingSides.bottom = true;
        if (tag.contains("Left"))
            blockingSides.left = true;
        if (tag.contains("Right"))
            blockingSides.right = true;

        // Create a directional collider
        sprite = new DirectionalEntityCollider(imagePath, blockingSides);
    }

    // If the tag is a Player
    else if (tag.startsWith("Player")) {
        // Create a player
        sprite = new Player(m_pCore);
    }

    // If the tag is a DashRefill
    else if (tag.startsWith("DashRefill")) {
        // Create a dash refill
        sprite = new DashRefill();
    }

    // If the tag is anything else
    else {
        // Create an AdvancedCollisionSprite
        auto* advSprite = new AdvancedCollisionSprite(imagePath);

        // Set the tag of the sprite as the collision tag
        advSprite->collisionTag = tag;

        sprite = advSprite;
    }
    return sprite;
}

//! Unloads the current level.
void LevelLoader::unloadLevel() {
    m_currentLevel = "";

    // Delete all sprites
    for (Sprite* sprite : m_pCore->scene()->sprites()) {
        m_pCore->scene()->removeSpriteFromScene(sprite);
        sprite->deleteLater();
    }
}

//! Reloads the current level.
//! This also functions as a "reset" function.
void LevelLoader::reloadCurrentLevel() {
    QString level = m_currentLevel;
    unloadLevel();
    loadLevel(level);
}

//! Applies parameters to a sprite.
//! Such as an animation to play.
//! \param pSprite The sprite to apply the parameters to.
//! \param params The parameters to apply.
void LevelLoader::applyParameters(Sprite* pSprite, const QString params) {
    QList<QString> parameterList = params.split("&");

    for (const QString& parameter : parameterList) {
        if (parameter.startsWith("Anim:")) {
            // Get animation times
            QList<QString> timesInString = parameter.split(":")[1].split(",");
            QList<int> times;

            // Convert to ints
            foreach (QString time, timesInString) {
                times.append(time.toInt());
            }

            // Create the animation in the sprite
            pSprite->createAnimation(pSprite->pixmap().toImage(), times);
            pSprite->startAnimation();
        }
    }
}

