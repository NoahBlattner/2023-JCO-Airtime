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
Player::Player(GameCore *gameCore) : PhysicsEntity(QDir::toNativeSeparators(GameFramework::resourcesPath() + "/images/player.png")) {
    // Connect the key events to the player
    connect(gameCore, &GameCore::notifyKeyPressed, this, &Player::onKeyPressed);
    connect(gameCore, &GameCore::notifyKeyReleased, this, &Player::onKeyReleased);
}

//! Player tick handler
//! Makes the player move on x axis based on the move direction
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick
void Player::tick(long long elapsedTimeInMilliseconds) {
    // set the x velocity based on the move direction
    setXVelocity(walkDirection * PLAYER_WALK_SPEED);

    // Call the parent tick handler
    PhysicsEntity::tick(elapsedTimeInMilliseconds);
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
