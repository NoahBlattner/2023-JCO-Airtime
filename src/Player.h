//
// Created by blatnoa on 22.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_PLAYER_H
#define INC_2023_JCO_AIRTIME_PLAYER_H

#include "PhysicsEntity.h"

#include <QVector2D>

class GameCore;

class Player : public PhysicsEntity {
    Q_OBJECT

public:
    explicit Player(GameCore* gamecore, QGraphicsItem* parent = nullptr);

    const float PLAYER_WALK_SPEED = 1;
    const float PLAYER_JUMP_SPEED = -3;
    const float PLAYER_STOP_TIME = .15;

    void tick(long long int elapsedTimeInMilliseconds) override;

private:
    float prevWalkDirection = 0;
    float walkDirection = 0;

    void jump();

private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);
};


#endif //INC_2023_JCO_AIRTIME_PLAYER_H
