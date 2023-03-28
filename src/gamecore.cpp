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
#include "DirectionalEntityCollision.h"

const int SCENE_WIDTH = 1920;

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
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));
    
    // Instancier et initialiser les sprite ici :
    auto* player = new Player(this);
    player->setPos(500, 0);
    m_pScene->addSpriteToScene(player);
    auto* player2 = new Player(this);
    player2->setPos(1000, 0);
    m_pScene->addSpriteToScene(player2);
    player2 -> addCollidingClass("Sprite");
    player2-> removeCollidingClass("AdvancedCollisionSprite");

    auto* plateforme1 = new Sprite(GameFramework::imagesPath() + "/plateform.png");
    plateforme1->setPos(100, 700);
    plateforme1-> setScale(.35);
    m_pScene->addSpriteToScene(plateforme1);

    auto* plateforme2 = new AdvancedCollisionSprite(GameFramework::imagesPath() + "/plateform.png");
    plateforme2->setPos(500, 700);
    plateforme2-> setScale(.35);
    m_pScene->addSpriteToScene(plateforme2);

    DirectionalEntityCollider::BlockingSides blockingSides = DirectionalEntityCollider::BlockingSides();
    blockingSides.top = false;
    auto* plateforme3 = new DirectionalEntityCollider(GameFramework::imagesPath() + "/plateform.png", blockingSides);
    plateforme3->setPos(1200, 700);
    plateforme3->setScale(.35);
    m_pScene->addSpriteToScene(plateforme3);

    QGraphicsTextItem* text;
    text = m_pScene->addText("Sprite", QFont("Arial", 30));
    text -> setDefaultTextColor(Qt::white);
    text->setPos(100, 600);
    text = m_pScene->addText("AdvancedCollisionSprite", QFont("Arial", 30));
    text -> setDefaultTextColor(Qt::white);
    text->setPos(500, 600);a
    text = m_pScene->addText("Directional", QFont("Arial", 30));
    text -> setDefaultTextColor(Qt::white);
    text->setPos(1200, 600);

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);

}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
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

