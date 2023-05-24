//
// Created by blatnoa on 16.05.2023.
//

#ifndef INC_2023_JCO_AIRTIME_PARTICLE_H
#define INC_2023_JCO_AIRTIME_PARTICLE_H

#include <QRandomGenerator64>
#include "AnimatedSprite.h"
#include "PhysicsEntity.h"

//! \brief A class that can be used to create particles
//!
//! This class is used to create particles.
//! This is useful for creating particles that can be used for explosions, smoke, dust, etc.
//!
//! The particle can be of a certain type.
//! The type can be used to create different behaviors for different types of particles.
//! Types:
//!     - DEFAULT: The default type. This type is used for particles that don't have a specific type. They will behave like any other physics entity.
//!     - EXPLOSIVE: This type is used for particles that are created by an explosion. These particles will be pushed away from their spawn location in a random direction.
//!     - SMOKE: This type is used for particles that are used for smoke. These particles will slowly fade away, while moving upwards.
//!     - DUST: This type is used for particles that are used for dust. These particles will slowly fade away, while moving downwards.
//!     - TRAVEL: This type is used for particles that are used for traveling. These particles will try to reach another sprite. When they reach the sprite, they will be destroyed.
//!
//! If the particle is of type TRAVEL, a target sprite needs to be set. This can be done with the setTravelTarget function.
//!
//! A particle contains multiple modifiers that can be used to change the behavior of the particle.
//! These modifiers can be used to change the speed, acceleration, etc.
//! These modifiers can be accessed directly.
//! Modifiers:
//!     - randomisation: The randomisation of the particle. This is the amount of randomness that is applied to the particle.
//!     - initialSpeed: The initial speed of the particle. This is the speed at which the particle is spawned.
//!     - acceleration: The acceleration of the particle. This is the acceleration at which the particle moves.
//!     - rotationSpeed: The rotation speed of the particle. This is the speed at which the particle rotates.
//!     - rotationAcceleration: The rotation acceleration of the particle. This is the acceleration at which the particle rotates.
//!     - fadeTime: The fade speed of the particle. This is the speed at which the particle fades.
class Particle : public PhysicsEntity {

public:
    enum ParticleType {
        DEFAULT,
        EXPLOSIVE,
        SMOKE,
        DUST,
        TRAVEL
    };

    Particle(ParticleType type, const QString& rImagePath, QGraphicsItem* pParent = nullptr);

    void setParentScene(GameScene* pScene) override;

    void setParticleType(ParticleType type);
    [[nodiscard]] inline ParticleType getParticleType() const { return particleType; };

    void setTravelTarget(Sprite* pTarget);

    // Modifiers
    float randomisation = 0.25f;
    float initialSpeed = 5.0f;
    float acceleration = 0;
    float rotationSpeed = 0.0f;
    float rotationAcceleration = 0.0f;
    float fadeTime = 2.0f;

protected:
    virtual void tick(long long int elapsedTimeInMilliseconds) override;

    virtual void onTrigger(AdvancedCollisionSprite* pOther) override;

private:
    ParticleType particleType = DEFAULT;
    Sprite* pTravelTarget = nullptr;
    bool deleteOnReach = false;
    bool targetReached = false;

    void (Particle::*updateFunction)(long long elapsedTimeInMilliseconds) = nullptr;

    void initParticle();

    void updateTravel(long long elapsedTimeInMilliseconds);
    void updateDefault(long long elapsedTimeInMilliseconds);

    void randomizeDirection(QVector2D &direction) const;

    void setRandomVelocity();
    void deleteOnFadeEnd();
};


#endif //INC_2023_JCO_AIRTIME_PARTICLE_H
