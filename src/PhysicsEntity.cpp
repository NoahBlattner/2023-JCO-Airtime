//
// Created by blatnoa on 22.03.2023.
//

#include "PhysicsEntity.h"

#include "GameScene.h"

PhysicsEntity::PhysicsEntity(QGraphicsItem* pParent) : AdvancedCollisionSprite(pParent) {};

PhysicsEntity::PhysicsEntity(const QString &rImagePath, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {};

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
    m_newRect = sceneBoundingRect().translated(moveVector.x(), moveVector.y());

    // Limit the rect to the scene
    limitToSceneRect(m_newRect);

    // Check for intersections with other sprites
    checkIntersects(m_newRect);

    // Set the new position
    setPos(m_newRect.topLeft());

    // Reevaluate if the entity is on the ground
    reevaluateGrounded();
}

//! Limits a RectF to the scene rect
//! \param rect The reference to the RectF
void PhysicsEntity::limitToSceneRect(QRectF &rect) const {
    // If the new position is outside the scene, move it back inside
    if (rect.left() < 0) {
        rect.setX(0);
    } else if (rect.right() > m_pParentScene ->sceneRect().right()) {
        rect.setX(m_pParentScene ->sceneRect().right() - rect.width());
    }

    if (rect.top() < 0) {
        rect.setY(0);
    } else if (rect.bottom() > m_pParentScene ->sceneRect().bottom()) {
        rect.setY(m_pParentScene ->sceneRect().bottom() - rect.height());
    }
}

//! Aligns a RectF to a sprite
//! \param newRect The reference to the RectF
//! \param sprite The sprite to align to
void PhysicsEntity::alignRectToSprite(QRectF &rect, const Sprite* pSprite) {
    if (pSprite) { // If the sprite is not null
        // Find the intersection between the new rect and the sprite
        QRectF intersection = m_newRect.intersected(pSprite->sceneBoundingRect());
        if (intersection.width() < intersection.height()) { // If the intersection is wider than it is tall
            if (x() < pSprite->x()) { // If the entity is to the left of the sprite
                m_newRect.setX(pSprite->sceneBoundingRect().left() - m_newRect.width());
            } else { // If the entity is to the right of the sprite
                m_newRect.setX(pSprite->sceneBoundingRect().right());
            }
        } else { // If the intersection is taller than it is wide
            if (y() < pSprite->y()) { // If the entity is above the sprite
                m_newRect.setY(pSprite->sceneBoundingRect().top() - m_newRect.height());
            } else { // If the entity is below the sprite
                m_newRect.setY(pSprite->sceneBoundingRect().bottom());
            }
        }
    }
}

//! Reevaluate if the entity is on the ground
//! This is done by checking if the entity is colliding with another sprite at the bottom
//! And if the entity is at the bottom of the scene
//! \return True if the entity is on the ground, false otherwise
bool PhysicsEntity::reevaluateGrounded() {
    // Check if the player is on the ground
    QRectF groundRect = sceneBoundingRect().translated(0, GROUNDED_DISTANCE);
    QList<Sprite*> collidingSprites = m_pParentScene->collidingSprites(groundRect);
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

//! Called when the entity collides with another sprite
//! Aligns the m_newRect to the other sprite
//! \param pOther The other sprite
void PhysicsEntity::onCollision(Sprite* pOther) {
    AdvancedCollisionSprite::onCollision(pOther);

    alignRectToSprite(m_newRect, pOther);
}
