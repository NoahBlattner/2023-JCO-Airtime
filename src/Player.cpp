//
// Created by blatnoa on 22.03.2023.
//

#include "Player.h"

#include "resources.h"
#include "GameCore.h"
#include "GameScene.h"
#include "AnimatedSprite.h"
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
    // Transitions
    startRunFrame = QPixmap(QDir::toNativeSeparators(GameFramework::imagesPath() + "/start-run.png"));
    startRunFrameFlipped = startRunFrame.transformed(QTransform().scale(-1, 1));

    // Other frames
    dustParticles = QPixmap(QDir::toNativeSeparators(GameFramework::imagesPath() + "/dust.png"));

    // Idle animation
    QImage image(QDir::toNativeSeparators(GameFramework::imagesPath() + "/idle-player.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(IDLE_ANIMATION_FRAME_DURATIONS));

    // Flipped idle animation
    image = QImage(QDir::toNativeSeparators(GameFramework::imagesPath() + "/idle-player-flipped.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(IDLE_ANIMATION_FRAME_DURATIONS));

    // Walk animation
    image = QImage(QDir::toNativeSeparators(GameFramework::imagesPath() + "/walk-player.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(WALK_ANIMATION_FRAME_DURATIONS));

    // Flipped walk animation
    image = QImage(QDir::toNativeSeparators(GameFramework::imagesPath() + "/walk-player-flipped.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(WALK_ANIMATION_FRAME_DURATIONS));

    // Jump animation
    image = QImage(QDir::toNativeSeparators(GameFramework::imagesPath() + "/jump-player.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(JUMP_ANIMATION_FRAME_DURATIONS));

    // Flipped jump animation
    image = QImage(QDir::toNativeSeparators(GameFramework::imagesPath() + "/jump-player-flipped.png"));
    createAnimation(image, QList<int>::fromReadOnlyData(JUMP_ANIMATION_FRAME_DURATIONS));

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
    bool previousGrounded = isOnGround();
    PhysicsEntity::reevaluateGrounded();

    if (isOnGround() && !isDashing) { // If the player is on the ground and not dashing
        // Reset the dash
        dashEnabled = true;

        if (!previousGrounded) { // If the player was not on the ground before
            // Show the dust particles
            showDustParticles();
        }
    }

    return isOnGround();
}

/**
 * Causes the player to die
 */
void Player::die() {
    emit notifyPlayerDied();
}

/**
 * Set the current animation
 * @param state The animation state to set
 */
void Player::setAnimation(Player::AnimationState state) {
    int newAnimIndex = 0;

    // Get the animation index according to the state
    switch (state) {
        case IDLE:
            newAnimIndex = playerFaceDirection > 0 ? 0 : 1;
            break;
        case WALK:
            if (currentAnimationState == IDLE) { // If the player was idle
                // Show the start run frame for a short duration to make the transition smoother
                showFrameFor(playerFaceDirection > 0 ? startRunFrame : startRunFrameFlipped, START_RUN_DURATION);
            }
            newAnimIndex = playerFaceDirection > 0 ? 2 : 3;
            break;
        case JUMP:
            stopAnimation(END_OF_CYCLE_STOP);
            newAnimIndex = playerFaceDirection > 0 ? 4 : 5;
            break;
        case DASH:
            newAnimIndex = playerFaceDirection > 0 ? 6 : 7;
            break;
        case DIE:
            newAnimIndex = playerFaceDirection > 0 ? 8 : 9;
            break;
    }

    if (currentAnimationState != state) { // If the animation state changed
        // Start the animation (in case it was stopped by JUMP animation)
        startAnimation();
    }

    // Set the new animation
    currentAnimationState = state;
    setActiveAnimation(newAnimIndex);
}

/**
 * Apply the walk input to the player according to the elapsed time
 * @param elapsedTimeInMilliseconds The elapsed time since the last tick
 */
void Player::applyWalkInput(long long int elapsedTimeInMilliseconds) {
    float prevXVelocity = velocity().x();

    // Calculate the new x velocity based on the move direction and the elapsed time
    float newXVelocity = prevXVelocity + inputDirection .x() * PLAYER_WALK_SPEED * elapsedTimeInMilliseconds / 1000.0f;
    // Clamp the new x velocity to the max walk speed
    newXVelocity = std::clamp<float>(newXVelocity, -PLAYER_WALK_SPEED, PLAYER_WALK_SPEED);

    // Adapt velocity
    if (isOnGround()) { // If the player is on the ground
        if (inputDirection.x() == 0 || // If the player is not walking
            inputDirection.x() * velocity().x() < 0) { // Or if the player is walking in the opposite direction

                // Slow down the player over time
                if (abs(newXVelocity) <= PLAYER_STOP_SPEED) { // If the player is slow enough to stop
                    newXVelocity = 0;
                    setAnimation(IDLE);
                } else {
                    // Slow down the player over time
                    newXVelocity -= (newXVelocity > 0 ? 1 : -1) * PLAYER_STOP_SPEED * elapsedTimeInMilliseconds / 1000.0f /
                                    PLAYER_STOP_TIME;
                }
        } else {
            // Set the animation to walking
            setAnimation(WALK);
        }
    } else { // If the player is in the air
        // Set the animation to jumping
        setAnimation(JUMP);
    }

    // Set the x velocity based on the move direction
    setXVelocity(newXVelocity);
}

/**
 * Shows dust particles at the bottom of the player
 */
void Player::showDustParticles() const {
    QPoint playerBottomCenter = QPoint(sceneBoundingRect().center().x(), sceneBoundingRect().bottom());

    auto* dust = new AnimatedSprite(dustParticles .toImage(), QList<int>::fromReadOnlyData(DUST_FRAME_DURATIONS));
    dust->setPos(playerBottomCenter - QPoint(dust->boundingRect().width() / 2, dust->boundingRect().height()));
    scene()->addItem(dust);
}

/*****************************
 * MOUVEMENT
 ****************************/

/**
 * Make the player walk based on the input direction and the elapsed time
 * @param elapsedTimeInMilliseconds
 */
void Player::walk(long long int elapsedTimeInMilliseconds) {
    // Get the new face direction if it changed
    if (playerFaceDirection * inputDirection.x() < 0) { // If the player is changing walking direction
        if (inputDirection.x() < 0) {
            playerFaceDirection = -1;
        } else {
            playerFaceDirection = 1;
        }
    }

    // Apply the walk input
    applyWalkInput(elapsedTimeInMilliseconds);
}

/**
 * Makes the player jump
 * Only works if the player is on the ground
 */
void Player::jump() {
    // If the player is on the ground
    if (isOnGround()) {
        setAnimation(JUMP);
        // Set the y velocity to make the player jump
        setYVelocity(PLAYER_JUMP_SPEED);

        // Show dust particles
        showDustParticles();
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

    // Get the dash velocity
    currentDashVector = direction.normalized() * PLAYER_DASH_SPEED - velocity();
    // Apply the dash velocity
    addVelocity(currentDashVector);

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

    QVector2D newVelocity = velocity() - currentDashVector;

    // Verify that the new velocity is not inverted
    // (The player has less speed than the dash added, for example if the player hit a wall)

    // If newVelocity x is inverted, set it to 0
    if (newVelocity.x() * velocity().x() <= 0) {
        newVelocity.setX(0);
    }

    // If newVelocity y is inverted, set it to 0
    if (newVelocity.y() * velocity().y() <= 0) {
        newVelocity.setY(0);
    }

    // Set the new velocity
    setVelocity(newVelocity);

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
