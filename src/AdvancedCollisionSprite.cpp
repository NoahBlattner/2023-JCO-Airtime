//
// Created by blatnoa on 27.03.2023.
//

#include "AdvancedCollisionSprite.h"

#include "GameScene.h"

AdvancedCollisionSprite::AdvancedCollisionSprite(QGraphicsItem* pParent) : Sprite(pParent) {}
AdvancedCollisionSprite::AdvancedCollisionSprite(const QString& rImagePath, QGraphicsItem* pParent) : Sprite(rImagePath, pParent) {}
AdvancedCollisionSprite::AdvancedCollisionSprite(const QString& rImagePath, QRectF collisionOverride, QGraphicsItem* pParent) : Sprite(rImagePath, pParent) {
    setCollisionOverride(collisionOverride);
}

//! Sets the collision override
//! The collision override is used instead of the scene bounding rect for collision detection
//! \param rect The rect to use as the new collision override
void AdvancedCollisionSprite::setCollisionOverride(QRectF rect) {
    collisionOverrideRect = rect;
}

//! Removes the collision override
void AdvancedCollisionSprite::removeCollisionOverride() {
    collisionOverrideRect = QRectF();
}

//! Returns the collision rect
//! \return The collision rect. If the collision override is empty, returns the scene bounding rect
QRectF AdvancedCollisionSprite::collisionRect() const {
    if (collisionOverrideRect.isEmpty()) { // If the collision override is empty
        return sceneBoundingRect();
    } else {
        // Base the collision rect on the collision override
        QRectF collisionRect = QRectF(0, 0 , collisionOverrideRect.width(), collisionOverrideRect.height());

        // Center the collision rect on the sprite
        collisionRect.moveCenter(sceneBoundingRect().center());

        // Move the collision rect to the collision override position
        collisionRect.translate(collisionOverrideRect.x(), collisionOverrideRect.y());
        return collisionRect;
    }
}

//! Get the collision rect of any sprite
//! If the sprite is an advanced collision sprite, use the collision rect instead of the scene bounding rect
//! \param pSprite The sprite to get the collision rect of
//! \return The collision rect of the sprite
QRectF AdvancedCollisionSprite::getCollisionRect(Sprite* pSprite) {
    QRectF otherCollisionRect = pSprite->sceneBoundingRect();

    auto* pAdvancedCollisionSprite = dynamic_cast<AdvancedCollisionSprite*>(pSprite);
    if (pAdvancedCollisionSprite) { // If the sprite is an advanced collision sprite
        // Use the collision rect instead of the scene bounding rect
        otherCollisionRect = pAdvancedCollisionSprite->collisionRect();
    }
    return otherCollisionRect;
}

//! Adds a tag to the colliding tags list
//! If the tag is "BlockAll", the list will be cleared and "BlockAll" will be added
//! If the list contains "BlockAll", it will be cleared and the tag will be added
//! \param tagName The tag name to add to the colliding tags list
void AdvancedCollisionSprite::addCollidingTag(const QString &tagName) {
    if (tagName == "BlockAll") { // If the tag is "BlockAll"
        collideAll();
    } else if (collidingTags.contains("All")) { // If the list contains "BlockAll"
        collidingTags.clear();
        collidingTags.append(tagName);
    } else {
        collidingTags.append(tagName);
    }
}

//! Sets the colliding tags list to contain only "BlockAll"
void AdvancedCollisionSprite::collideAll() {
    collidingTags.clear();
    collidingTags.append("BlockAll");
}

//! Checks for intersections with other sprites
//! Uses the current collision rect if it is not empty
//! Else uses the current scene bounding rect
void AdvancedCollisionSprite::reevaluateIntersects() {
    reevaluateIntersects(collisionRect());
}

//! Checks for intersections with other sprites
//! Uses the given rect
//! \param rect The rect to check for intersections with
void AdvancedCollisionSprite::reevaluateIntersects(QRectF rect) {
    auto collidingSprites= getCollidingSprites(rect);

    foreach (AdvancedCollisionSprite* pSprite, collidingSprites) { // For each colliding sprite
        if (pSprite->isTrigger) { // If the other sprite is a trigger
            // Call its onTrigger function
            pSprite->onTrigger(this);
        } else { // If the sprite is a collision
            onCollision(pSprite);
        }
    }
}

//! Returns a list of AdvancedCollisionSprites that the sprite is colliding with
//! Only returns AdvancedCollisionSprites that have the same collision tag as this sprite
//! \param rect The rect to check for intersections with. If empty, uses the current scene bounding rect of the sprite
//! \return A list of AdvancedCollisionSprites that the sprite is colliding with
QList<AdvancedCollisionSprite*> AdvancedCollisionSprite::getCollidingSprites(QRectF rect) const {
    auto collidingSprites = m_pParentScene->collidingSprites((rect.isEmpty()) ? collisionRect() : rect);
    collidingSprites.removeAll(this);

    QList<AdvancedCollisionSprite*> advCollidingSprites;

    foreach (Sprite* pSprite, collidingSprites) { // For each colliding sprite
        auto* pAdvancedCollisionSprite = dynamic_cast<AdvancedCollisionSprite*>(pSprite);
        if (pAdvancedCollisionSprite) { // If the sprite is an advanced collision sprite
            advCollidingSprites << pAdvancedCollisionSprite;
        }
    }

    if (collidingTags.contains("BlockAll")) { // If the sprite collides with all sprites
        return advCollidingSprites;
    }

    // Remove sprites that are not in the colliding classes list
    for (int i = 0; i < advCollidingSprites.size(); i++) {
        auto* advSprite = dynamic_cast<AdvancedCollisionSprite*>(advCollidingSprites[i]);
        if (!advSprite || !collidingTags.contains(collisionTag)) {
            advCollidingSprites.removeAt(i);
        }
    }

    return advCollidingSprites;
}

//! Called when the sprites intersects with another sprite as a trigger
//! Emits the notifyTrigger signal
//! \param pOther The other sprite
void AdvancedCollisionSprite::onTrigger(AdvancedCollisionSprite* pOther) {
    emit notifyTrigger(pOther);
}

//! Called when the sprites intersects with another sprite as a collision
//! Emits the notifyCollision signal
//! \param pOther The other sprite
void AdvancedCollisionSprite::onCollision(AdvancedCollisionSprite* pOther) {
    emit notifyCollision(pOther);
}
