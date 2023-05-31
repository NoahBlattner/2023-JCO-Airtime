//
// Created by blatnoa on 16.05.2023.
//

#include "Particle.h"

#include "GameScene.h"
#include <utility>
#include <QRandomGenerator>

//! Constructor :
//! Removes collisions from the particle.
//! \param type The type of the particle.
//! \param animationSpriteSheet The spritesheet of the particle.
//! \param frameDurations The durations of each frame of the animation.
//! \param pParent The parent of the particle.
Particle::Particle(Particle::ParticleType type, const QString& rImagePath, QGraphicsItem* pParent)
: PhysicsEntity(rImagePath, pParent) {
    isTrigger = true;

    collidingTags.clear();

    // Set the particle type
    setParticleType(type);

    friction = 0;
}

//! Set the travel target for the TRAVEL particle type.
//! \param pTarget The target of the particle.
void Particle::setTravelTarget(Sprite* pTarget) {
    pTravelTarget = pTarget;

    // Listen to the destroyed signal of the target
    connect(pTravelTarget, &Sprite::destroyed, this, [this]() {
        // When the target is destroyed, remove the target pointer
        pTravelTarget = nullptr;
        deleteLater();
    });
}

//! Override of the setParentScene function.
//! Initializes the particle.
void Particle::setParentScene(GameScene* pScene) {
    PhysicsEntity::setParentScene(pScene);

    initParticle();
}

//! Delete the particle after the fade time.
void Particle::deleteOnFadeEnd() {
    // Set the particle to be deleted after the fade time
    QTimer::singleShot(fadeTime * 1000, this, [this]() {
        deleteLater();
    });
}

//! Initialize the particle.
void Particle::initParticle() {

    setRandomVelocity();

    switch (particleType) {
        case EXPLOSIVE:
            setGravityEnabled(false);
            deleteOnFadeEnd();

            break;
        case SMOKE:
            gravity = 0.5f;
            friction = 0;

            deleteOnFadeEnd();

            // Set the initial y velocity to be upwards
            setYVelocity(-initialSpeed);
            break;

        case DUST:
            gravity = -0.5f;
            friction = 0;

            deleteOnFadeEnd();

            // Set the initial y velocity to be downwards
            setYVelocity(initialSpeed);
            break;

        case TRAVEL:
            setGravityEnabled(false);

            // Initially, the particle is not deleted when it reaches its destination
            deleteOnReach = false;
            QTimer::singleShot(fadeTime * 1000, this, [this]() {
                // After the fade time, the particle is deleted when it reaches its target
                deleteOnReach = true;
            });

            break;
    }
}

//! Set a random velocity for the particle.
void Particle::setRandomVelocity() {
    float randRange = initialSpeed * randomisation;
    float randX = QRandomGenerator::global()->generateDouble() * randRange - randRange / 2.0f;
    float randY = QRandomGenerator::global()->generateDouble() * randRange - randRange / 2.0f;

    setVelocity(QVector2D(randX,
                          randY));
}

//! Update the particle type
//! \param type The new particle type
void Particle::setParticleType(Particle::ParticleType type) {
    particleType = type;

    // Set the update function
    switch (type) {
        case DEFAULT:
            updateFunction = nullptr;
            break;
        case EXPLOSIVE:
        case SMOKE:
        case DUST:
            updateFunction = &Particle::updateDefault;
            break;
        case TRAVEL:
            updateFunction = &Particle::updateTravel;
            break;
    }
}

//! Override of the tick function.
//! Calls the particle update function.
void Particle::tick(long long elapsedTimeInMilliseconds) {
    // Call the update function
    if (updateFunction != nullptr) {
        (this->*updateFunction)(elapsedTimeInMilliseconds);
    }

    PhysicsEntity::tick(elapsedTimeInMilliseconds);
}

//! Update function for particles of type TRAVEL.
//! Moves the particle towards the target.
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick.
void Particle::updateTravel(long long elapsedTimeInMilliseconds) {
    // Check if the target is valid
    if (pTravelTarget == nullptr) {
        // If no target, delete the particle
        deleteLater();
        return;
    }

    // Calculate the direction
    QVector2D direction = QVector2D(pTravelTarget->pos() - pos());

    randomizeDirection(direction);

    // Lerp new and old velocity
    float lerpFactor = acceleration * elapsedTimeInMilliseconds / 1000.0f;
    QVector2D velocity = velocityVector * (1.0f - lerpFactor) + direction.normalized() * initialSpeed * lerpFactor;

    setVelocity(velocity);
}

//! Slightly randomize the direction of the particle.
//! This is used to make the particles look more natural.
//! It does not completely randomize the direction, but only slightly changes it.
//! \param direction The direction to randomize.
void Particle::randomizeDirection(QVector2D &direction) const {// Randomise the direction
    QVector2D newDirection = direction.normalized();

    newDirection.setX(newDirection.x() + QRandomGenerator::global()->generateDouble() * randomisation - randomisation / 2.0f);
    newDirection.setY(newDirection.y() + QRandomGenerator::global()->generateDouble() * randomisation - randomisation / 2.0f);

    newDirection *= direction.length();

    direction = newDirection;
}

//! Update function for most particles (except TRAVEL).
//! Randomizes the direction and decreases the opacity to make the particle fade out.
//! Also applies acceleration.
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick.
void Particle::updateDefault(long long int elapsedTimeInMilliseconds) {
    // Randomise the direction
    randomizeDirection(velocityVector);

    // Set opacity
    setOpacity(opacity() - (float)elapsedTimeInMilliseconds / (fadeTime * 1000.0f));

    // Add acceleration
    velocityVector += velocityVector.normalized() * acceleration * elapsedTimeInMilliseconds / 1000.0f;
}

//! Override of the onTrigger function.
//! Deletes the particle if it is of type TRAVEL and the target is reached.
void Particle::onTrigger(AdvancedCollisionSprite* pOther) {
    AdvancedCollisionSprite::onTrigger(pOther);

    if (particleType == TRAVEL && deleteOnReach && pOther == pTravelTarget) {
        // If the target is reached, delete the particle
        deleteLater();
    }
}

//! Set the acceleration of the particle.
//! If the particle is of type TRAVEL, the acceleration is limited between 0 and 1.
//! \param acceleration The new acceleration.
void Particle::setAcceleration(float acceleration) {
    this->acceleration = acceleration;

    if (particleType == TRAVEL) {
        // Limit the acceleration between 0 and 1
        this->acceleration = std::min(1.0f, std::max(0.0f, this->acceleration));
    }
}
