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
//! On construction, the player is given a set of animations and a set of collision classes
//! The player is also given a set of physics overrides
//! Key events are connected to the player from a GameCore instance to allow the player to move
//!
//! The player is always automatically registered for ticks when the parent scene is set
//!
//! The player's animations are updated according to the current properties of the player
//!
//! The class contains a series of constants that can be used to tweak the player's movement
class Player : public PhysicsEntity {
    Q_OBJECT

public:
    explicit Player(GameCore* gamecore, QGraphicsItem* parent = nullptr);

    const QRectF PLAYER_COLLISION_RECT = QRectF(0, 5, 56, 150);
    const float PLAYER_GRAVITY_OVERRIDE = -12;
    const float PLAYER_FRICTION_OVERRIDE = .75;
    const float PLAYER_WALK_SPEED = 2;
    const float PLAYER_JUMP_SPEED = -4;
    const float PLAYER_STOP_SPEED = .5;
    const float PLAYER_STOP_TIME = .2;

    void tick(long long int elapsedTimeInMilliseconds) override;

private:
    float prevWalkDirection = 0;
    float walkDirection = 0;

    // Array of animation frame durations for the idle animation
    const int IDLE_ANIMATION_FRAME_DURATIONS[10] = {2000, 100,1500, 100, 1500, 100, 100, 2500, 100, 100};

    void jump();

private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);

    void initAnimations();
};


#endif //INC_2023_JCO_AIRTIME_PLAYER_H
