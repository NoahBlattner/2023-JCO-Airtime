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

    // Player constants
    const QRectF PLAYER_COLLISION_RECT = QRectF(0, 5, 56, 150);
    const float PLAYER_GRAVITY_OVERRIDE = -12;
    const float PLAYER_FRICTION_OVERRIDE = .75;
    const float PLAYER_WALK_SPEED = 2;
    const float PLAYER_JUMP_SPEED = -2.5;
    const float PLAYER_DASH_SPEED = 4;
    const float PLAYER_DASH_TIME = .1;
    const float PLAYER_STOP_SPEED = .75;
    const float PLAYER_STOP_TIME = .3;

    void tick(long long int elapsedTimeInMilliseconds) override;

    virtual void onCollision(AdvancedCollisionSprite* pOther) override;

    bool reevaluateGrounded() override;

private:
    QVector2D inputDirection = QVector2D(0, 0);

    void applyPressedKeys(GameCore* gamecore);

    // Animations
    enum AnimationState {
        IDLE, WALK, JUMP, DASH, DIE
    };
    AnimationState currentAnimationState = IDLE;
    void setAnimation(AnimationState state);
    // Transmission frames
    QPixmap startRunFrame;
    QPixmap startRunFrameFlipped;
    const int START_RUN_DURATION = 100;
    // Array of animation frame durations for the idle animation
    const int IDLE_ANIMATION_FRAME_DURATIONS[12] = {2000, 100,1500, 100, 1500, 100, 100, 100, 2500, 100, 100, 100};
    const int WALK_ANIMATION_FRAME_DURATIONS[8] = {50, 50, 50, 50, 50, 50, 50, 50};

    // Movement
    void jump();
    // Dash
    QTimer dashTimer;
    QVector2D currentDashVector = QVector2D(0, 0);
    bool dashEnabled = true;
    bool isDashing = false;
    void dash(QVector2D direction);
    void endDash();

    int playerFaceDirection = 1;

    void die();

signals:
    void notifyPlayerDied();

private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);

    void initAnimations();

    void walk(long long int elapsedTimeInMilliseconds);

    void applyWalkInput(long long int elapsedTimeInMilliseconds);
};


#endif //INC_2023_JCO_AIRTIME_PLAYER_H
