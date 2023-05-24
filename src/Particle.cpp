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

//! Initialize the particle.
void Particle::initParticle() {
    switch (particleType) {
        case TRAVEL:
            // Initially, the particle is not deleted when it reaches its destination
            deleteOnReach = false;
            QTimer::singleShot(fadeTime * 1000, this, [this]() {
                // After the fade time, the particle is deleted when it reaches its target
                deleteOnReach = true;
            });

            // Set a random starting velocity
            float randRange = initialSpeed * randomisation;
            float randX = QRandomGenerator::global()->generateDouble() * randRange - randRange / 2.0f;
            float randY = QRandomGenerator::global()->generateDouble() * randRange - randRange / 2.0f;

            setVelocity(QVector2D(randX,
                                  randY));
            break;
    }
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
            updateFunction = &Particle::updateExplosive;
            break;
        case SMOKE:
            updateFunction = &Particle::updateSmoke;
            break;
        case DUST:
            updateFunction = &Particle::updateDust;
            break;
        case TRAVEL:
            setGravityEnabled(false);
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

    // Normalize the direction
    direction.normalize();

    // Randomise the directionw
    direction.setX(direction.x() + QRandomGenerator::global()->generateDouble() * randomisation - randomisation / 2.0f);
    direction.setY(direction.y() + QRandomGenerator::global()->generateDouble() * randomisation - randomisation / 2.0f);

    // Lerp new and old velocity
    float lerpFactor = acceleration * elapsedTimeInMilliseconds / 1000.0f;
    QVector2D velocity = velocityVector * (1.0f - lerpFactor) + direction * initialSpeed * lerpFactor;

    setVelocity(velocity);
}

//! Update function for particles of type EXPLOSIVE.
//! Moves the particle away from the spawn point in a random direction.
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick.
void Particle::updateExplosive(long long int elapsedTimeInMilliseconds) {
    // TODO
}

//! Update function for particles of type SMOKE.
//! Moves the particle upwards.
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick.
void Particle::updateSmoke(long long int elapsedTimeInMilliseconds) {
    // TODO
}

//! Update function for particles of type DUST.
//! Moves the particle slowly downwards.
//! \param elapsedTimeInMilliseconds The elapsed time since the last tick.
void Particle::updateDust(long long int elapsedTimeInMilliseconds) {
    // TODO
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
