//
// Created by blatnoa on 24.05.2023.
//

#include "MovingPlatform.h"
#include "resources.h"

//! Constructor :
//! Creates a moving platform.
//! \param moveVector The vector the platform will startMove in.
//! \param moveDuration The duration of the movement.
//! \param pParent The parent of the platform.
MovingPlatform::MovingPlatform(QVector2D moveVector, float moveDuration, QGraphicsItem* pParent) : PhysicsEntity(pParent) {
    setPixmap(QPixmap(GameFramework::imagesPath() + "plateform.png"));

    this->moveVector = moveVector;
    this->moveDuration = moveDuration;
}

//! Tick handler :
//! Moves the platform if it is moving.
//! \param elapsedTimeInMilliseconds The elapsed time in milliseconds.
void MovingPlatform::tick(long long int elapsedTimeInMilliseconds) {
    if (moving) {
        moveTime += elapsedTimeInMilliseconds;

        if (moveTime >= moveDuration) {
            // Move the platform to the end position
            move(moveVector * (moveDuration - (moveTime - elapsedTimeInMilliseconds) / elapsedTimeInMilliseconds));
            endMove();
        } else {
            // Move the platform by a fraction of the move vector
            move(moveVector * elapsedTimeInMilliseconds / moveDuration);
        }
    }
}

//! Ends the movement of the platform.
void MovingPlatform::endMove() {
    moveTime = 0;
    moving = false;
    moveVector *= -1;

    if (direction == FORTH) {
        direction = BACK;

        // Wait 1 second before moving back
        QTimer::singleShot(1000, this, SLOT(startMove()));
    } else {
        direction = FORTH;
    }
}

//! Override of the onSteppedOn function.
//! Triggered when an entity steps on the platform.
//! Starts the movement of the platform.
//! \param pEntity The entity that stepped on the platform.
void MovingPlatform::onSteppedOn(PhysicsEntity* pEntity) {
    pEntity->setParent(this);

    if (direction == BACK || moving) { // If the platform is already moving or will move back,
        // Do nothing
        return;
    }
    startMove();
}

//! Starts the movement of the platform.
void MovingPlatform::startMove() {
    moving = true;
    moveTime = 0;
}
