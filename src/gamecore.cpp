/**
  Fichier qui contient toute la logique du jeu.
  
  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <cmath>

#include <QDebug>
#include <QSettings>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "Player.h"
#include "LevelLoader.h"

const int SCENE_WIDTH = 3500;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {
    
    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;
    
    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);
    
    // Trace un rectangle blanc tout autour des limites de la scène.
    // m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));

    levelLoader = new LevelLoader(this, GameFramework::resourcesPath() + "levels");
    levelLoader->loadLevel("mainLevel");

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le débogueur est démarré.
    m_pGameCanvas->startTick();
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

/**
 * Changes the current level
 * @param levelName The name of the level to load
 */
void GameCore::loadLevel(QString levelName) {
    levelLoader->loadLevel(levelName);
}

/**
 * Called when the player dies
 */
void GameCore::onPlayerDeath() {
    qDebug() << "Player died";
    playerHasDied = true;
}

/**
 * Resets the game
 */
void GameCore::resetLevel() {
    levelLoader->reloadCurrentLevel();
    playerHasDied = false;
}

/**
 * Resets the registered keys
 * All currently pressed keys are forced to be released
 */
void GameCore::resetKeys() {
    foreach (int key, m_pressedKeys) {
        keyReleased(key);
    }
    m_pressedKeys.clear();
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);

    switch (key) {
        case Qt::Key_Escape:
            m_pGameCanvas->stopTick();
            break;
        case Qt::Key_R:
            resetLevel();
            break;
    }

    m_pressedKeys.append(key);
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);

    m_pressedKeys.removeAll(key);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
    if (playerHasDied) { // If the player has died during the last tick,
        // Reset the game
        resetLevel();
    }
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}

