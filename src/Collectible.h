/**
\file     Collectible.h
\brief    Déclaration de la classe Collectible.
\author   Blattner Noah
\date     Mai 2023
*/
#ifndef INC_2023_JCO_AIRTIME_COLLECTIBLE_H
#define INC_2023_JCO_AIRTIME_COLLECTIBLE_H

class Player;

#include "AdvancedCollisionSprite.h"

//! \brief An abstract class that can be subclassed to create collectibles.
//!
//! This class is used to create collectibles by subclassing it.
//! It is automatically set to be a trigger.
//! When the trigger is triggered, the onCollect function is called before the trigger is destroyed.
//! The onCollect function is called when the collectible is intersecting with a player.
//! The onCollect function must be overridden to create custom behavior when the collectible is collected by a player.
class Collectible : public AdvancedCollisionSprite {

    explicit Collectible(const QString& rImagePath, QGraphicsItem* pParent = nullptr);
    Collectible(const QString& rImagePath, QRectF collisionOverride, QGraphicsItem* pParent = nullptr);

    virtual void onCollect(Player* player) = 0;

    void onTrigger(AdvancedCollisionSprite* pOther) override;
};


#endif //INC_2023_JCO_AIRTIME_COLLECTIBLE_H
