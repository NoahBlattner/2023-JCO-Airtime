//
// Created by blatnoa on 22.03.2023.
//

#include "Player.h"

#include "resources.h"
#include "GameCore.h"
#include "GameScene.h"
#include <QDir>
#include <QKeyEvent>

//! Constructor
//! \param gameCore The game core which sends the key events
Player::Player(GameCore *gameCore, QGraphicsItem *parent) : PhysicsEntity(parent) {
    // Create animations
    // Idle animation
    QImage image(QDir::toNativeSeparators(GameFramework::imagesPath() + "/idle-player.png"));
    createAnimation(image, IDLE_ANIMATION_FRAME_DURATIONS);

    startAnimation();

    // Set friction
    friction = PLAYER_FRICTION_OVERRIDE;
    gravity = PLAYER_GRAVITY_OVERRIDE;

    // Set collisions
    addCollidingClass("AdvancedCollisionSprite");

    // Connect the key events to the player
    connect(gameCore, &GameCore::notifyKeyPressed, this, &Player::onKeyPressed);
    connect(gameCore, &GameCore::notifyKeyReleased, this, &Player::onKeyReleased);
}

//! Player tick handler.
//! Updates the player x velocity based on inputs to make it move
//! Updates the animation of the player
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick
void Player::tick(long long elapsedTimeInMilliseconds) {
    float prevXVelocity = velocity().x();

    // Calculate the new x velocity based on the move direction and the elapsed time
    float newXVelocity = prevXVelocity + walkDirection * PLAYER_WALK_SPEED * elapsedTimeInMilliseconds / 1000.0f;
    // Clamp the new x velocity to the max walk speed
    newXVelocity = std::clamp<float>(newXVelocity, -PLAYER_WALK_SPEED, PLAYER_WALK_SPEED);

    // Adapt velocity
    if (walkDirection == 0 && isOnGround()) { // If the player is not walking
        if (abs(newXVelocity) <= PLAYER_STOP_SPEED) { // If the player is slow enough to stop
            newXVelocity = 0;
        } else {
            // Slow down the player over time
            newXVelocity -= (newXVelocity > 0 ? 1 : -1) * PLAYER_STOP_SPEED * elapsedTimeInMilliseconds / 1000.0f / PLAYER_STOP_TIME;
        }
    }

    // Set the x velocity based on the move direction
    setXVelocity(newXVelocity);

    if ((prevWalkDirection <= 0 && walkDirection > 0) ||
        (prevWalkDirection >= 0 && walkDirection < 0)) { // If the player is changing walking direction
        setActiveAnimation((walkDirection < 0) ? 1 : 0); // Change the animation
    }

    // Remember the move direction
    prevWalkDirection = walkDirection;

    // Call the parent tick handler which applies the velocity
    PhysicsEntity::tick(elapsedTimeInMilliseconds);

    parentScene() -> centerViewOn(this);
}

/*****************************
 * MOUVEMENT
 ****************************/

//! Makes the player jump
//! Only works if the player is on the ground
void Player::jump() {
    // If the player is on the ground
    if (isOnGround()) {
        // Set the y velocity to make the player jump
        setYVelocity(PLAYER_JUMP_SPEED);
    }
}

/*****************************
 * SLOTS
 ****************************/

//! Slot for key pressed events
//! \param event The key event
void Player::onKeyPressed(int key) {
    switch (key) {
        // Adapt the move direction based on the key pressed
        case Qt::Key_A:
            walkDirection -= 1;
            break;
        case Qt::Key_D:
            walkDirection += 1;
            break;
        case Qt::Key_Space:
            jump();
            break;
    }
}

//! Slot for key released events
//! \param event The key event
void Player::onKeyReleased(int key) {
    switch (key) {
        // Adapt the move direction based on the key released
        case Qt::Key_A:
            walkDirection += 1;
            break;
        case Qt::Key_D:
            walkDirection -= 1;
            break;
    }
}
