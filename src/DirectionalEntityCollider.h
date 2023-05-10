/**
\file     DirectionalEntityCollider.h
\brief    Déclaration de la classe DirectionalEntityCollider.
\author   Blattner Noah
\date     avril 2023
*/

#ifndef INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLIDER_H
#define INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLIDER_H


#include "AdvancedCollisionSprite.h"
#include "PhysicsEntity.h"

//! A class that can be used to block entities from moving in certain directions
//!
//! This class is used to block entities from moving in certain directions.
//! This is useful for creating walls and other objects that entities can collide with.
//!
//! This class is a subclass of AdvancedCollisionSprite,
//! so it can be used to create advanced collision all while blocking entities from moving in certain directions.
//!
//! The class contains a struct called BlockingSides that contains four bools.
//! These bools represent the four directions that the entity can move in.
//! If the bool is true, the entity is blocked from moving through the Sprite in that direction.
//! This field is public, so it can be accessed and modified directly.
//!
//! The class also contains a function called isEntityBlocked.
//! This function takes a pointer to a PhysicsEntity as a parameter.
//! The function returns true if the entity is blocked from moving in the direction it is moving.
class DirectionalEntityCollider : public AdvancedCollisionSprite {

    Q_OBJECT

public:
    struct BlockingSides {
        bool top = false;
        bool bottom = false;
        bool left = false;
        bool right = false;
    };

    explicit DirectionalEntityCollider(QGraphicsItem* pParent = nullptr);
    explicit DirectionalEntityCollider(const QString& rImagePath, QGraphicsItem* pParent = nullptr);
    DirectionalEntityCollider(const QString& rImagePath, BlockingSides blockingSides, QGraphicsItem* pParent = nullptr);

    BlockingSides m_blockingSides;

    bool isEntityBlocked(const PhysicsEntity* pEntity) const;

};


#endif //INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLIDER_H
