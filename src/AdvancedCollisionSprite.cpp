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

//! Adds a class to the colliding classes list
//! If the class is "All", the list will be cleared and "All" will be added
//! If the list contains "All", it will be cleared and the class will be added
//! \param className The class name to add to the colliding classes list
void AdvancedCollisionSprite::addCollidingClass(const QString &className) {
    if (className == "All") { // If the class is "All"
        collideAll();
    } else if (collidingClasses.contains("All")) { // If the list contains "All"
        collidingClasses.clear();
        collidingClasses.append(className);
    } else {
        collidingClasses.append(className);
    }
}

//! Sets the colliding classes list to contain only "All"
void AdvancedCollisionSprite::collideAll() {
    collidingClasses.clear();
    collidingClasses.append("All");
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

    foreach (Sprite* pSprite, collidingSprites) { // For each colliding sprite
        auto* pAdvancedCollisionSprite = dynamic_cast<AdvancedCollisionSprite*>(pSprite);
        if (pAdvancedCollisionSprite != nullptr && pAdvancedCollisionSprite->isTrigger) { // If the other sprite is a trigger
            // Call its onTrigger function
            pAdvancedCollisionSprite->onTrigger(this);
        } else { // If the sprite is a collision
            onCollision(pSprite);
        }
    }
}

//! Returns a list of sprites that the sprite is colliding with
//! Only returns sprites that are in the colliding classes list
//! \param rect The rect to check for intersections with. If empty, uses the current scene bounding rect of the sprite
//! \return A list of sprites that the sprite is colliding with
QList<Sprite*> AdvancedCollisionSprite::getCollidingSprites(QRectF rect) const {
    auto collidingSprites = m_pParentScene->collidingSprites((rect.isEmpty()) ? collisionRect() : rect);
    collidingSprites.removeAll(this);

    if (collidingClasses.contains("All")) { // If the sprite collides with all sprites
        return collidingSprites;
    }

    // Remove sprites that are not in the colliding classes list
    for (int i = 0; i < collidingSprites.size(); i++) {
        if (!collidingClasses.contains(collidingSprites[i]->metaObject()->className())) {
            collidingSprites.removeAt(i);
        }
    }

    return collidingSprites;
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
