//
// Created by blatnoa on 22.03.2023.
//

#include "PhysicsEntity.h"

#include "GameScene.h"

PhysicsEntity::PhysicsEntity(QGraphicsItem* pParent) : Sprite(pParent) {};

PhysicsEntity::PhysicsEntity(const QString &rImagePath, QGraphicsItem* pParent) : Sprite(rImagePath, pParent) {};

//! Set the parent scene
//! Automatically registers the player for ticks when the scene is set
//! \param pScene The parent scene
void PhysicsEntity::setParentScene(GameScene *pScene) {
    Sprite::setParentScene(pScene);

    // Register the player for ticks
    registerForTick();
}

//! Tick handler
//! Applies gravity and moves the entity based on the velocity and the elapsed time
//! \param elapsedTimeInMilliseconds The elapsed time in milliseconds
void PhysicsEntity::tick(long long elapsedTimeInMilliseconds) {
    // If gravity is enabled, apply it
    if (gravityEnabled) {
        if (m_isOnGround && velocityVector.y() > 0) { // If the player is on the ground and moving down
            // Remove the y velocity
            velocityVector.setY(0);
        } else {
            // Apply gravity
            velocityVector.setY(velocityVector.y() - gravity * elapsedTimeInMilliseconds / 1000.0f);
        }
    }

    // Apply friction
    velocityVector *= 1 - friction * elapsedTimeInMilliseconds / 1000.0f;

    // Move the player
    move(velocity() * elapsedTimeInMilliseconds);
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

    reevaluateGrounded();
}

//! Reevaluate if the entity is on the ground
//! This is done by checking if the entity is colliding with another sprite at the bottom
//! And if the entity is at the bottom of the scene
//! \return True if the entity is on the ground, false otherwise
bool PhysicsEntity::reevaluateGrounded() {
    // Check if the player is on the ground
    QRectF newRect = sceneBoundingRect().translated(0, GROUNDED_DISTANCE);
    QList<Sprite*> collidingSprites = m_pParentScene->collidingSprites(newRect);
    collidingSprites.removeAll(this);

    if (!collidingSprites.empty() // If the player is colliding with another sprite at the bottom
        || sceneBoundingRect().bottom() >= m_pParentScene->sceneRect().bottom() - GROUNDED_DISTANCE) { // Or if the player is at the bottom of the scene
        // The player is on the ground
        m_isOnGround = true;
    } else {
        // The player is not on the ground
        m_isOnGround = false;
    }

    return m_isOnGround;
}
