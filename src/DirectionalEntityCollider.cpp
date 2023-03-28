//
// Created by blatnoa on 27.03.2023.
//

#include "DirectionalEntityCollision.h"

DirectionalEntityCollider::DirectionalEntityCollider(QGraphicsItem* pParent) : AdvancedCollisionSprite(pParent) {}
DirectionalEntityCollider::DirectionalEntityCollider(const QString& rImagePath, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {}
DirectionalEntityCollider::DirectionalEntityCollider(const QString& rImagePath, BlockingDirections directions, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {
    m_directions = directions;
}

bool DirectionalEntityCollider::isEntityBlocked(PhysicsEntity* pEntity) const {
    // Check if the entity is moving
    if (pEntity->velocity().isNull()) {
        return false;
    }

    // Check if the entity is moving up
    if (pEntity->velocity().y() > 0) {
        // Check if the entity is blocked from moving up
        if (!m_directions.up) {
            return true;
        }
    }

    // Check if the entity is moving down
    if (pEntity->velocity().y() < 0) {
        // Check if the entity is blocked from moving down
        if (!m_directions.down) {
            return true;
        }
    }

    // Check if the entity is moving left
    if (pEntity->velocity().x() < 0) {
        // Check if the entity is blocked from moving left
        if (!m_directions.left) {
            return true;
        }
    }

    // Check if the entity is moving right
    if (pEntity->velocity().x() > 0) {
        // Check if the entity is blocked from moving right
        if (!m_directions.right) {
            return true;
        }
    }

    return false;
}

