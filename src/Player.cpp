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
    float prevXDirection = velocity().x();
    float newXDirection = walkDirection * PLAYER_WALK_SPEED;

    // set the x velocity based on the move direction
    setXVelocity(newXDirection);

    if ((prevXDirection <= 0 && newXDirection > 0) ||
        (prevXDirection >= 0 && newXDirection < 0)) { // If the player is changing walking direction
        setActiveAnimation((walkDirection < 0) ? 1 : 0); // Change the animation
    }

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
