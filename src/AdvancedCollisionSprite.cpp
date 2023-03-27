//
// Created by blatnoa on 27.03.2023.
//

#include "AdvancedCollisionSprite.h"

#include "GameScene.h"

AdvancedCollisionSprite::AdvancedCollisionSprite(QGraphicsItem* pParent) : Sprite(pParent) {}
AdvancedCollisionSprite::AdvancedCollisionSprite(const QString& rImagePath, QGraphicsItem* pParent) : Sprite(rImagePath, pParent) {}

//! Checks for intersections with other sprites
//! Uses the current scene bounding rect of the sprite
void AdvancedCollisionSprite::checkIntersects() {
    checkIntersects(sceneBoundingRect());
}

//! Checks for intersections with other sprites
//! Uses the given rect
//! \param rect The rect to check for intersections with
void AdvancedCollisionSprite::checkIntersects(QRectF rect) {
    // Check if the entity is colliding withamp any other sprites than the entity
    QList<Sprite*> collidingSprites = m_pParentScene->collidingSprites(rect);
    collidingSprites.removeAll(this);

    foreach (Sprite* pSprite, collidingSprites) { // For each colliding sprite
        if (isTrigger) { // If the sprite is a trigger
            onTrigger(pSprite);
        } else { // If the sprite is a collision
            onCollision(pSprite);
        }
    }
}

//! Called when the sprites intersects with another sprite as a trigger
//! Emits the notifyTrigger signal
//! \param pOther The other sprite
void AdvancedCollisionSprite::onTrigger(Sprite* pOther) {
    emit notifyTrigger(pOther);
}

//! Called when the sprites intersects with another sprite as a collision
//! Emits the notifyCollision signal
//! \param pOther The other sprite
void AdvancedCollisionSprite::onCollision(Sprite* pOther) {
    emit notifyCollision(pOther);
}