//
// Created by blatnoa on 27.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLISION_H
#define INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLISION_H


#include "AdvancedCollisionSprite.h"

class DirectionalEntityCollision : public AdvancedCollisionSprite {

    Q_OBJECT

public:
    struct Directions {
        bool up = true;
        bool down = true;
        bool left = true;
        bool right = true;
    };

    explicit DirectionalEntityCollision(QGraphicsItem* pParent = nullptr);
    explicit DirectionalEntityCollision(const QString& rImagePath, QGraphicsItem* pParent = nullptr);
    DirectionalEntityCollision(const QString& rImagePath, Directions directions, QGraphicsItem* pParent = nullptr);

    Directions m_directions;

protected:
    void onCollision(Sprite* pOther) override;

};


#endif //INC_2023_JCO_AIRTIME_DIRECTIONALENTITYCOLLISION_H
