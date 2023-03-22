//
// Created by blatnoa on 22.03.2023.
//

#include "PhysicsEntity.h"

#include "GameScene.h"

PhysicsEntity::PhysicsEntity(const QString &rImagePath, QGraphicsItem* pParent) : Sprite(rImagePath, pParent) {}

//! Set the parent scene
//! Automatically registers the player for ticks when the scene is set
//! \param pScene The parent scene
void PhysicsEntity::setParentScene(GameScene *pScene) {
    Sprite::setParentScene(pScene);

    // Register the player for ticks
    registerForTick();
}

//! Tick handler
//! Applies gravity and moves the entity
//! \param elapsedTimeInMilliseconds The elapsed time in milliseconds
void PhysicsEntity::tick(long long elapsedTimeInMilliseconds) {
    // If gravity is enabled, apply it
    if (m_gravityEnabled) {
        m_velocity.setY(m_velocity.y() + GRAVITY * elapsedTimeInMilliseconds / 1000.0f);
    }

    // Calculate the move vector
    QVector2D moveVector = velocity();
    moveVector *= elapsedTimeInMilliseconds;

    // Move the player
    move(moveVector);
}

//! Move the entity by a given vector
//! This movement is blocked by other sprites and the scene boundaries
//! \param moveVector The vector to move the entity by
void PhysicsEntity::move(QVector2D moveVector) {
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

    // Check if the new position is colliding with any other sprites than the entity
    QList<Sprite*> collidingSprites = m_pParentScene->collidingSprites(newRect);
    collidingSprites.removeAll(this);

    if (!collidingSprites.empty()) {
        // If there are collisions, align the entity with the colliding sprite
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
