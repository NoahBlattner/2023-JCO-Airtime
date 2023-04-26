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
    initAnimations();

    // Apply physics overrides
    friction = PLAYER_FRICTION_OVERRIDE;
    gravity = PLAYER_GRAVITY_OVERRIDE;

    // Set collisions
    collisionTag = "Player";
    setCollisionOverride(PLAYER_COLLISION_RECT);

    // Init timer
    dashTimer.setSingleShot(true);
    dashTimer.setInterval(PLAYER_DASH_TIME * 1000);
    connect(&dashTimer, &QTimer::timeout, this, &Player::endDash);

    // Connect the key events to the player
    connect(gameCore, &GameCore::notifyKeyPressed, this, &Player::onKeyPressed);
    connect(gameCore, &GameCore::notifyKeyReleased, this, &Player::onKeyReleased);
    connect(this, &Player::notifyPlayerDied, gameCore, &GameCore::onPlayerDeath);

    // Apply the already pressed keys
    applyPressedKeys(gameCore);
}

/**
 * Apply the pressed keys to the player
 * This method can be used to apply the already pressed keys when the player is created
 * @param gameCore The game core which sends the key events
 */
void Player::applyPressedKeys(GameCore* gameCore) {
    foreach (int key, gameCore->pressedKeys()) {
        onKeyPressed(key);
    }
}

//! Initialize the player animations
void Player::initAnimations() {
    // Idle animation
    QImage image(QDir::toNativeSeparators(GameFramework::imagesPath() + "/idle-player.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(IDLE_ANIMATION_FRAME_DURATIONS));

    // Flipped idle animation
    image = QImage(QDir::toNativeSeparators(GameFramework::imagesPath() + "/idle-player-flipped.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(IDLE_ANIMATION_FRAME_DURATIONS));

    startAnimation();
}

//! Player tick handler.
//! Updates the player x velocity based on inputs to make it move
//! Updates the animation of the player
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick
void Player::tick(long long elapsedTimeInMilliseconds) {
    if (!isDashing) {
        walk(elapsedTimeInMilliseconds);
    }

    // Call the parent tick handler which applies the velocity
    PhysicsEntity::tick(elapsedTimeInMilliseconds);

    parentScene()->centerViewOn(this);
}

/**
 * Override of the onCollision method
 * Handles the different event caused by a collision within the player
 * @param other The other sprite that collided with this one
 */
void Player::onCollision(AdvancedCollisionSprite* other) {
    PhysicsEntity::onCollision(other);

    // If the other is a death zone
    if (other->collisionTag == "KillZone") {
        // Kill the player
        die();
    }
}

/**
 * Reevaluate the grounded state of the player
 * Reset the dash if the player is grounded
 * @return
 */
bool Player::reevaluateGrounded() {
    PhysicsEntity::reevaluateGrounded();

    if (isOnGround() && !isDashing) { // If the player is on the ground and not dashing
        // Reset the dash
        dashEnabled = true;
    }

    return isOnGround();
}

/**
 * Causes the player to die
 */
void Player::die() {
    emit notifyPlayerDied();
}

/*****************************
 * MOUVEMENT
 ****************************/

/**
 * Make the player walk based on the input direction and the elapsed time
 * @param elapsedTimeInMilliseconds
 */
void Player::walk(long long int elapsedTimeInMilliseconds) {
    float prevXVelocity = velocity().x();

    // Calculate the new x velocity based on the move direction and the elapsed time
    float newXVelocity = prevXVelocity + inputDirection .x() * PLAYER_WALK_SPEED * elapsedTimeInMilliseconds / 1000.0f;
    // Clamp the new x velocity to the max walk speed
    newXVelocity = std::clamp<float>(newXVelocity, -PLAYER_WALK_SPEED, PLAYER_WALK_SPEED);

    // Adapt velocity
    if (inputDirection .x() == 0 && isOnGround()) { // If the player is not walking
        if (abs(newXVelocity) <= PLAYER_STOP_SPEED) { // If the player is slow enough to stop
            newXVelocity = 0;
        } else {
            // Slow down the player over time
            newXVelocity -= (newXVelocity > 0 ? 1 : -1) * PLAYER_STOP_SPEED * elapsedTimeInMilliseconds / 1000.0f /
                            PLAYER_STOP_TIME;
        }
    }

    // Set the x velocity based on the move direction
    setXVelocity(newXVelocity);

    if ((prevWalkDirection <= 0 && inputDirection.x() > 0) ||
        (prevWalkDirection >= 0 && inputDirection.x() < 0)) { // If the player is changing walking direction
        if (inputDirection.x() < 0) {
            playerFaceDirection = -1;
            setActiveAnimation(1); // Change the animation
        } else {
            playerFaceDirection = 1;
            setActiveAnimation(0); // Change the animation
        }
    }

    // Remember the move direction
    prevWalkDirection = inputDirection.x();
}

/**
 * Makes the player jump
 * Only works if the player is on the ground
 */
void Player::jump() {
    // If the player is on the ground
    if (isOnGround()) {
        // Set the y velocity to make the player jump
        setYVelocity(PLAYER_JUMP_SPEED);
    }
}

/**
 * Makes the player dash in a directionRotation
 * This temporarily disables gravity and friction
 * The dash is ended after PLAYER_DASH_TIME seconds
 * The dash is not allowed if the player is already dashing or if the dash is not enabled
 * @param directionRotation The directionRotation to dash in
 */
void Player::dash(QVector2D direction) {
    if (isDashing || !dashEnabled) { // Do not allow dashing if already dashing
        return;
    }

    isDashing = true;
    dashEnabled = false;

    if (direction.isNull()) { // If the direction is null
        // Dash in the directionRotation the player is facing
        direction = QVector2D(playerFaceDirection, 0);
    }

    // Apply the dash velocity
    currentDashVector = direction.normalized() * PLAYER_DASH_SPEED;
    setVelocity(currentDashVector);

    // Disable gravity and friction
    setGravityEnabled(false);
    friction = 0;

    // Start the timer to end the dash
    dashTimer.start();
}

/**
 * Ends the dash by removing the dash velocity and re-enabling gravity and friction
 */
void Player::endDash() {
    isDashing = false;

    // Remove the dash velocity
    addVelocity(-currentDashVector);
    currentDashVector = QVector2D(0, 0);

    // Re-enable gravity and friction
    setGravityEnabled(true);
    friction = PLAYER_FRICTION_OVERRIDE;
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
            inputDirection -= QVector2D(1, 0);
            break;
        case Qt::Key_D:
            inputDirection += QVector2D(1, 0);
            break;
        case Qt::Key_W:
            inputDirection -= QVector2D(0, 1);
            break;
        case Qt::Key_S:
            inputDirection += QVector2D(0, 1);
            break;
        case Qt::Key_Space:
            jump();
            break;
        case Qt::Key_Shift:
            dash(inputDirection);
            break;
    }
}

//! Slot for key released events
//! \param event The key event
void Player::onKeyReleased(int key) {
    switch (key) {
        // Adapt the move direction based on the key released
        case Qt::Key_A:
            inputDirection += QVector2D(1, 0);
            break;
        case Qt::Key_D:
            inputDirection -= QVector2D(1, 0);
            break;
        case Qt::Key_W:
            inputDirection += QVector2D(0, 1);
            break;
        case Qt::Key_S:
            inputDirection -= QVector2D(0, 1);
            break;
    }
}
