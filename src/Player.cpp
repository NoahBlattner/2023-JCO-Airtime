//
// Created by blatnoa on 22.03.2023.
//

#include "Player.h"

#include "resources.h"
#include "GameCore.h"
#include "PlayerTickHandler.h"
#include "GameScene.h"
#include <QDir>
#include <QKeyEvent>

//! Constructor
//! \param gameCore The game core which sends the key events
Player::Player(GameCore *gameCore) : Sprite(QDir::toNativeSeparators(GameFramework::resourcesPath() + "/images/player.png")) {
    // Create and set the player tick handler
    setTickHandler(new PlayerTickHandler());

    // Connect the key events to the player
    connect(gameCore, &GameCore::notifyKeyPressed, this, &Player::onKeyPressed);
    connect(gameCore, &GameCore::notifyKeyReleased, this, &Player::onKeyReleased);
}

//! Set the parent scene
//! Automatically registers the player for ticks
//! \param pScene The parent scene
void Player::setParentScene(GameScene *pScene) {
    Sprite::setParentScene(pScene);

    // Register the player for ticks
    registerForTick();
}

//! Move the player by a given vector
//! This movement is blocked by walls
//! \param moveVector The vector to move the player by
void Player::move(QVector2D moveVector) {
    // Calculate the new position
    QRectF newRect = sceneBoundingRect().translated(moveVector.x(), moveVector.y());

    // If the new position is outside the scene, move it back inside
    if (newRect.left() < 0) {
        newRect.setX(0);
    } else if (newRect.right() > m_pParentScene->sceneRect().right()) {
        newRect.setX(m_pParentScene->sceneRect().right() - newRect.width());
    }

    if (newRect.top() < 0) {
        newRect.setY(0);
    } else if (newRect.bottom() > m_pParentScene->sceneRect().bottom()) {
        newRect.setY(m_pParentScene->sceneRect().bottom() - newRect.height());
    }

    // Check if the new position is colliding with any other sprites than the player
    QList<Sprite*> collidingSprites = m_pParentScene->collidingSprites(newRect);
    collidingSprites.removeAll(this);

    if (!collidingSprites.empty()) {
        // If there are collisions, align the player with the colliding sprite
        Sprite* collidingSprite = dynamic_cast<Sprite*>(collidingSprites.first());
        if (collidingSprite) {
            if (moveVector.x() > 0) {
                newRect.setX(collidingSprite->sceneBoundingRect().left() - newRect.width());
            } else if (moveVector.x() < 0) {
                newRect.setX(collidingSprite->sceneBoundingRect().right());
            }

            if (moveVector.y() > 0) {
                newRect.setY(collidingSprite->sceneBoundingRect().top() - newRect.height());
            } else if (moveVector.y() < 0) {
                newRect.setY(collidingSprite->sceneBoundingRect().bottom());
            }
        }
    }

    // Set the new position
    setPos(newRect.topLeft());
}

/*****************************
 * SLOTS
 ****************************/

//! Slot for key pressed events
//! \param event The key event
void Player::onKeyPressed(int key) {
    switch (key) {
        // Adapt the move direction based on the key pressed
        case Qt::Key_W:
            m_moveDirection += QVector2D(0, -1);
            break;
        case Qt::Key_S:
            m_moveDirection += QVector2D(0, 1);
            break;
        case Qt::Key_A:
            m_moveDirection += QVector2D(-1, 0);
            break;
        case Qt::Key_D:
            m_moveDirection += QVector2D(1, 0);
            break;
    }
}

//! Slot for key released events
//! \param event The key event
void Player::onKeyReleased(int key) {
    switch (key) {
        // Adapt the move direction based on the key released
        case Qt::Key_W:
            m_moveDirection -= QVector2D(0, -1);
            break;
        case Qt::Key_S:
            m_moveDirection -= QVector2D(0, 1);
            break;
        case Qt::Key_A:
            m_moveDirection -= QVector2D(-1, 0);
            break;
        case Qt::Key_D:
            m_moveDirection -= QVector2D(1, 0);
            break;
    }
}
