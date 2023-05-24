//
// Created by blatnoa on 24.05.2023.
//

#ifndef INC_2023_JCO_AIRTIME_MOVINGPLATFORM_H
#define INC_2023_JCO_AIRTIME_MOVINGPLATFORM_H


#include "PhysicsEntity.h"

class MovingPlatform : public PhysicsEntity {

public:
    MovingPlatform(QVector2D moveVector, float moveDuration, QGraphicsItem* pParent = nullptr);

    void onSteppedOn(PhysicsEntity* pEntity) override;

    void tick(long long int elapsedTimeInMilliseconds) override;

    enum Direction {
        FORTH,
        BACK
    };

private:
    QVector2D moveVector;
    Direction direction = FORTH;
    float moveDuration;
    float moveTime = 0;
    bool moving = false;

    void startMove();

    void endMove();
};


#endif //INC_2023_JCO_AIRTIME_MOVINGPLATFORM_H
