//
// Created by blatnoa on 22.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_PLAYER_H
#define INC_2023_JCO_AIRTIME_PLAYER_H

#include "PhysicsEntity.h"

#include <QVector2D>

class GameCore;

//! The player entity inherits from the PhysicsEntity class
//! This class handles the player input and applies the appropriate velocity to the player
//! The player is also affected by gravity and friction
//!
//! The player is always automatically registered for ticks when the parent scene is set
//!
//! The class manages the player's walk direction
//! And allows the player to jump
//!
//! The player's animations are updated according to the properties of the player
class Player : public PhysicsEntity {
    Q_OBJECT

public:
    explicit Player(GameCore* gamecore, QGraphicsItem* parent = nullptr);

    const float PLAYER_GRAVITY = -12;
    const float PLAYER_WALK_SPEED = 2;
    const float PLAYER_JUMP_SPEED = -4;
    const float PLAYER_STOP_SPEED = .5;
    const float PLAYER_STOP_TIME = .2;
    const float PLAYER_FRICTION = .75;

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
