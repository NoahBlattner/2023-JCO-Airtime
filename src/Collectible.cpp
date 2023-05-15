//
// Created by blatnoa on 15.05.2023.
//

#include "gamescene.h"

#include "Collectible.h"

Collectible::Collectible(const QString &rImagePath, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {
    isTrigger = true;
}

Collectible::Collectible(const QString &rImagePath, QRectF collisionOverride, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, collisionOverride, pParent) {
    isTrigger = true;
}

//! Override of the onTrigger function from AdvancedCollisionSprite:
//! When the collectible is intersecting with a player, the onCollect function is called.
//! The collectible is then destroyed.
void Collectible::onTrigger(AdvancedCollisionSprite* pOther) {
    AdvancedCollisionSprite::onTrigger(pOther);

    if (pOther->collisionTag == "Player") {
        // Collectible was collected by player
        onCollect((Player*) pOther);
    }

    // Remove from scene
    scene()->removeItem(this);
    delete this;
}
