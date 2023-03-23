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

    const int PLAYER_WALK_SPEED = 1;
    const int PLAYER_JUMP_SPEED = -3;

    void tick(long long int elapsedTimeInMilliseconds) override;

private:
    float walkDirection = 0;

    void jump();

private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);
};


#endif //INC_2023_JCO_AIRTIME_PLAYER_H
