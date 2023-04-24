/*
 * @file LevelLoader.cpp
 * @brief Définition de la classe LevelLoader.
 * @author Noah Blattner
 * @date Février 2023
 */

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

//! Constructeur
//! \param core Le gamecore
//! \param levelsPath Le chemin des niveaux
LevelLoader::LevelLoader(GameCore* core, QString levelsPath) {
    m_pCore = core;
    m_levelsPath = std::move(levelsPath);
}

//! Charge un niveau dans la scène
//! \param scene La scène dans laquelle charger le niveau
//! \param levelName Le nom du niveau
//! \return La liste des sprites chargés
QList<Sprite *> LevelLoader::loadLevel(const QString& levelName) {
    // Concaténation du chemin du niveau avec le nom du niveau
    QString levelPath = m_levelsPath + "/" + levelName;

    if (!levelName.endsWith(".json")) { // Si le nom du niveau ne finit pas par .json
        levelPath += ".json";
    }

    levelPath = QDir::toNativeSeparators(levelPath);

    if (!QFile::exists(levelPath)) { // Si le fichier n'existe pas
        // On affiche une erreur
        QMessageBox::critical(nullptr, "Erreur", "Le fichier de niveau " + levelName + " n'existe pas.");
        return {};
    }

    QFile file(levelPath);
    if (!file.open(QIODevice::ReadOnly)) { // Si on ne peut pas ouvrir le fichier
        // On affiche une erreur
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier de niveau " + levelName + ".");
        return {};
    }

    // On prend les données du fichier et on le ferme
    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
    file.close();

    // On récupère l'objet JSON
    QJsonObject levelObject = jsonDocument.object();

    // On charge l'arrière-plan
    m_pCore->scene()->setBackgroundImage(QImage(GameFramework::imagesPath() + levelObject["background"].toString()));

    // On adapte la taille de la scène
    m_pCore->scene()->setSceneRect(0, 0, levelObject["sceneWidth"].toInt(), levelObject["sceneHeight"].toInt());

    // On charge les sprites
    return loadSprites(levelObject["sprites"].toArray());
}

//! Charge les sprites dans la scène
//! \param scene La scène dans laquelle charger les sprites
//! \param spritesArray Le tableau JSON des sprites
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

/**
 * Load a sprite from a JSON object.
 * Loads a subclass of Sprite if specified in the tag.
 * @param spriteObject The JSON object containing the sprite data.
 * @return The loaded sprite.
 */
Sprite* LevelLoader::loadSprite(const QJsonObject &spriteObject) {
    // On crée la sprite avec son image
    Sprite* sprite = nullptr;

    QString tag = spriteObject["tag"].toString();
    QString imagePath = QDir::toNativeSeparators(GameFramework::imagesPath() +
            spriteObject["textureName"].toString());

    qDebug() << "Loading sprite " << imagePath << " with tag " << tag;

    if (tag == "Player") {
        // On crée une sprite de type Player
        sprite = new Player(m_pCore);
    } else if (tag.startsWith("DirectionalCollider")) {
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
    } else if (tag.isEmpty()) {
        // Create a simple sprite
        sprite = new Sprite(imagePath);
    } else {
        // Create an AdvancedCollisionSprite
        auto* advSprite = new AdvancedCollisionSprite(imagePath);

        // Set the tag of the sprite as the collision tag
        advSprite->collisionTag = tag;

        sprite = advSprite;
    }

    // On applique les transformations
    sprite->setTransformOriginPoint(sprite->globalBoundingRect().center());
    sprite->setPos(spriteObject["x"].toDouble(), spriteObject["y"].toDouble());
    sprite->setScale(spriteObject["scale"].toDouble());
    sprite->setRotation(spriteObject["rotation"].toInt());

    m_pCore->scene()->addSpriteToScene(sprite); // On ajoute la sprite à la scène

    return sprite;
}

//! Décharge un niveau de la scène
void LevelLoader::unloadLevel() {
    for (Sprite* sprite : m_pCore->scene()->sprites()) {
        m_pCore->scene()->removeSpriteFromScene(sprite);
        delete sprite;
    }
}
