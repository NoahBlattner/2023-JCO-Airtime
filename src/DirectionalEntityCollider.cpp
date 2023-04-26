//
// Created by blatnoa on 27.03.2023.
//

#include "DirectionalEntityCollider.h"

DirectionalEntityCollider::DirectionalEntityCollider(QGraphicsItem* pParent) : AdvancedCollisionSprite(pParent) {}
DirectionalEntityCollider::DirectionalEntityCollider(const QString& rImagePath, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {
    collisionTag = "BlockAll";
}
DirectionalEntityCollider::DirectionalEntityCollider(const QString& rImagePath, BlockingSides blockingSides, QGraphicsItem* pParent) : DirectionalEntityCollider(rImagePath, pParent) {
    m_blockingSides = blockingSides;
}

//! Checks if an entity is blocked from moving in the direction it is moving by this Sprite
//! @param pEntity A pointer to the entity to check
//! @return True if the entity is blocked from moving in the direction it is moving by this Sprite
bool DirectionalEntityCollider::isEntityBlocked(const PhysicsEntity* pEntity) const {
    // Check if the entity is moving
    if (pEntity->velocity().isNull()) {
        return true;
    }

    // Check if the entity is moving up
    if (pEntity->velocity().y() > 0) {
        // Check if the entity is blocked from moving up
        if (!m_blockingSides.bottom) {
            return true;
        }
    }

    // Check if the entity is moving down
    if (pEntity->velocity().y() < 0) {
        // Check if the entity is blocked from moving down
        if (!m_blockingSides.top) {
            return true;
        }
    }

    // Check if the entity is moving left
    if (pEntity->velocity().x() < 0) {
        // Check if the entity is blocked from moving left
        if (!m_blockingSides.right) {
            return true;
        }
    }

    // Check if the entity is moving right
    if (pEntity->velocity().x() > 0) {
        // Check if the entity is blocked from moving right
        if (!m_blockingSides.left) {
            return true;
        }
    }

    return false;
}

