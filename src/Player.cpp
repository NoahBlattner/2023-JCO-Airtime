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
    // Set animations
    addAnimationFrame(QPixmap(QDir::toNativeSeparators(GameFramework::resourcesPath() + "/images/player.png")));
    addAnimation();
    setActiveAnimation(1);
    addAnimationFrame(QPixmap(QDir::toNativeSeparators(GameFramework::resourcesPath() + "/images/player-flip.png")));
    startAnimation();

    // Connect the key events to the player
    connect(gameCore, &GameCore::notifyKeyPressed, this, &Player::onKeyPressed);
    connect(gameCore, &GameCore::notifyKeyReleased, this, &Player::onKeyReleased);
}

//! Player tick handler
//! Makes the player move on x axis based on the move direction
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick
void Player::tick(long long elapsedTimeInMilliseconds) {
    float prevXVelocity = velocity().x();
    float newXVelocity = prevXVelocity;

    if (walkDirection == 0) { // If the player is not moving
        // Slow down the player within a certain time
        if (prevXVelocity > 0)
            newXVelocity -= PLAYER_WALK_SPEED * elapsedTimeInMilliseconds / 1000.0f / PLAYER_STOP_TIME;
        else if (prevXVelocity < 0)
            newXVelocity += PLAYER_WALK_SPEED * elapsedTimeInMilliseconds / 1000.0f / PLAYER_STOP_TIME;
    } else {
        // Calculate the new x velocity based on the move direction and the elapsed time
        newXVelocity = prevXVelocity + walkDirection * PLAYER_WALK_SPEED * elapsedTimeInMilliseconds / 1000.0f;
        // Clamp the new x velocity to the max walk speed
        newXVelocity = std::clamp<float>(newXVelocity, -PLAYER_WALK_SPEED, PLAYER_WALK_SPEED);

        if ((prevWalkDirection <= 0 && walkDirection > 0) ||
            (prevWalkDirection >= 0 && walkDirection < 0)) { // If the player is changing walking direction
            setActiveAnimation((walkDirection < 0) ? 1 : 0); // Change the animation

            // Give the player a little boost in the opposite direction
            // In order to avoid to much sliding
            newXVelocity += walkDirection * PLAYER_WALK_SPEED / 1.1;
        }

    }
    // Set the x velocity based on the move direction
    setXVelocity(newXVelocity);

    // Remember the move direction
    prevWalkDirection = walkDirection;

    // Call the parent tick handler
    PhysicsEntity::tick(elapsedTimeInMilliseconds);
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
