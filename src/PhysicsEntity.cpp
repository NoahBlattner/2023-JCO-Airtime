//
// Created by blatnoa on 22.03.2023.
//

#include "PhysicsEntity.h"

#include "GameScene.h"
#include "DirectionalEntityCollider.h"

PhysicsEntity::PhysicsEntity(QGraphicsItem* pParent) : AdvancedCollisionSprite(pParent) {
    addCollidingClass("DirectionalEntityCollider");
};

PhysicsEntity::PhysicsEntity(const QString &rImagePath, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {
    addCollidingClass("DirectionalEntityCollider");
};

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
    m_newRect = collisionRect().translated(moveVector.x(), moveVector.y());

    // Limit the rect to the scene
    limitRectToScene(m_newRect);

    // Check for intersections with other sprites
    reevaluateIntersects(m_newRect);

    // Translate the entity to the new position
    setPos(pos() + m_newRect.topLeft() - collisionRect().topLeft());

    // Reevaluate if the entity is on the ground
    reevaluateGrounded();
}

//! Returns a list of sprites that the sprite is colliding with
//! Only returns sprites that are in the colliding classes list
//! And only returns directional entity colliders that are currently blocking the entity
//! \param rect The rect to check for intersections with. If empty, uses the current scene bounding rect of the sprite
//! \return A list of sprites that the sprite is colliding with
QList<Sprite*> PhysicsEntity::getCollidingSprites(QRectF rectF) const {
    auto collidingSprites = AdvancedCollisionSprite::getCollidingSprites(rectF);

    // Remove directional entity colliders that are not blocking the entity
    foreach (auto* pSprite, collidingSprites) {
        auto pCollider = dynamic_cast<DirectionalEntityCollider*>(pSprite);

        // If the collider is not blocking the entity, remove it from the list
        if (pCollider && !pCollider->isEntityBlocked(this)) {
            collidingSprites.removeOne(pSprite);
        }
    }

    return collidingSprites;
}

//! Limits a RectF to the scene rect
//! \param rect The reference to the RectF
void PhysicsEntity::limitRectToScene(QRectF &rect) const {
    // If the given rect is outside the scene, move it back inside
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
void PhysicsEntity::alignRectToSprite(QRectF &rect, Sprite* pSprite) {
    if (pSprite) { // If the sprite is not null
        QRectF otherCollisionRect = getCollisionRect(pSprite);

        // Find the intersection between the new rect and the sprite
        QRectF intersection = rect.intersected(otherCollisionRect);
        if (intersection.width() < intersection.height()) { // If the intersection is wider than it is tall
            if (x() < pSprite->x()) { // If the entity is to the left of the sprite
                rect.setX(otherCollisionRect.left() - rect.width());
            } else { // If the entity is to the right of the sprite
                rect.setX(otherCollisionRect.right());
            }
            // Remove the x velocity
            setVelocity(QVector2D(0, velocity().y()));
        } else { // If the intersection is taller than it is wide
            if (y() < pSprite->y()) { // If the entity is above the sprite
                rect.setY(otherCollisionRect.top() - rect.height());
            } else { // If the entity is below the sprite
                rect.setY(otherCollisionRect.bottom());
            }
            // Remove the y velocity
            setVelocity(QVector2D(velocity().x(), 0));
        }
    }
}

//! Reevaluate if the entity is on the ground
//! This is done by checking if the entity is colliding with another sprite at the bottom
//! And if the entity is at the bottom of the scene
//! \return True if the entity is on the ground, false otherwise
bool PhysicsEntity::reevaluateGrounded() {
    // Check if the player is on the ground
    QRectF groundRect = collisionRect().translated(0, GROUNDED_DISTANCE);
    QList<Sprite*> groundCheckCollisions = getCollidingSprites(groundRect);

    // Determine if the player is on the ground
    if (!groundCheckCollisions.empty() // If the player is colliding with another sprite at the bottom
        || collisionRect().bottom() >= m_pParentScene->sceneRect().bottom() - GROUNDED_DISTANCE) { // Or if the player is at the bottom of the scene
        // The player is on the ground
        m_isOnGround = true;
    } else {
        // The player is not on the ground
        m_isOnGround = false;
    }

    return m_isOnGround;
}

//! Called when the entity collides with another sprite
//! If the collider is a directional entity collider and the entity is not blocked by it, the collision is ignored
//! Otherwise, the parent onCollision function is called and the entity is aligned to the other sprite
//! \param pOther The other sprite
void PhysicsEntity::onCollision(Sprite* pOther) {
    auto* directionalCollider = dynamic_cast<DirectionalEntityCollider*>(pOther);
    if (directionalCollider != nullptr &&
        !directionalCollider->isEntityBlocked(this)) { // If the other sprite is a directional entity collider and the entity is not blocked by it
        // Ignore the collision
        return;
    }

    // Call the parent onCollision function
    AdvancedCollisionSprite::onCollision(pOther);

    // Align the m_newRect to the other sprite
    alignRectToSprite(m_newRect, pOther);
}