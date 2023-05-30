//
// Created by blatnoa on 24.05.2023.
//

#ifndef INC_2023_JCO_AIRTIME_MOVINGPLATFORM_H
#define INC_2023_JCO_AIRTIME_MOVINGPLATFORM_H


#include "PhysicsEntity.h"

//! \brief A class that can be used to create moving platforms.
//!
//! This class is used to create moving platforms.
//!
//! It is activated when a player steps on it.
//! When activated, it will move in a direction over a certain amount of time.
//! When it reaches the end of its path, it will move back to its original position.
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
