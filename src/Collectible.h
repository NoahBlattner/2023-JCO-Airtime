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
//! When the trigger is intersecting with a player, the onCollect function is called.
//! The onCollect function must be overridden to create custom behavior when the collectible is collected by a player.
//!
//! The collectible can be set to respawn after a certain amount of time.
//! The collectible will be hidden and disabled when it is collected and will reappear after the specified amount of time.
//! If the respawn time is set to 0, the collectible will not respawn but will be destroyed when collected.
class Collectible : public AdvancedCollisionSprite {

protected:
    explicit Collectible(unsigned int respawnTime = 0, QGraphicsItem* pParent = nullptr);
    explicit Collectible(const QString& rImagePath, unsigned int respawnTime = 0, QGraphicsItem* pParent = nullptr);
    Collectible(const QString& rImagePath, QRectF collisionOverride, unsigned int respawnTime = 0, QGraphicsItem* pParent = nullptr);

    virtual void onCollect(Player* player);

    void onTrigger(AdvancedCollisionSprite* pOther) override;

private:
    unsigned int m_respawnTime = 0;
    GameScene* m_pScene = nullptr;

    void spawnCollectParticles(Player* pPlayer, int particleCount = 5);

    void disable();

private slots:
    void enable();
};


#endif //INC_2023_JCO_AIRTIME_COLLECTIBLE_H
