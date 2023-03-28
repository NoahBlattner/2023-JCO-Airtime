//
// Created by blatnoa on 27.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLISION_H
#define INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLISION_H


#include "AdvancedCollisionSprite.h"
#include "PhysicsEntity.h"

class DirectionalEntityCollider : public AdvancedCollisionSprite {

    Q_OBJECT

public:
    struct Directions {
        bool up = true;
        bool down = true;
        bool left = true;
        bool right = true;
    };

    explicit DirectionalEntityCollider(QGraphicsItem* pParent = nullptr);
    explicit DirectionalEntityCollider(const QString& rImagePath, QGraphicsItem* pParent = nullptr);
    DirectionalEntityCollider(const QString& rImagePath, Directions directions, QGraphicsItem* pParent = nullptr);

    Directions m_directions;

    bool isEntityBlocked(PhysicsEntity* pEntity);

};


#endif //INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLISION_H
