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
//! Most of these modifiers are accessible through the public variables of the particle. (except for the acceleration)
//! Modifiers:
//!     - randomisation: The randomisation of the particle. This is the amount of randomness that is applied to the particle.
//!                      Initial speed will be modified to range from initialSpeed - randomisation to initialSpeed + randomisation.
//!                      On each tick, the direction of the particle will be modified by a random amount between -randomisation and +randomisation.
//!     - initialSpeed: The initial speed of the particle. This is the speed at which the particle is spawned.
//!     - acceleration: The acceleration of the particle. This is the acceleration at which the particle moves.
//!                     A general acceleration that is applied to the particle as currentVelocity += acceleration * elapsedTimeInMilliseconds / 1000.0f.
//!                     Except for particles of type TRAVEL. For these particles, the acceleration is used to lerp the travel speed and the pull towards the target. (Limited to 0.0f - 1.0f)
//!     - fadeTime: The fade speed of the particle. This is the speed at which the particle fades.
//!                 The particle will fade from 1.0f to 0.0f in fadeTime seconds.
//!                 Particles of type TRAVEL use this modifier to determine how long the particle will have to exist before it can be deleted by reaching its target.
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
    float fadeTime = 2.0f;
    void setAcceleration(float acceleration);
    [[nodiscard]] inline float getAcceleration() const { return acceleration; };

protected:
    virtual void tick(long long int elapsedTimeInMilliseconds) override;

    virtual void onTrigger(AdvancedCollisionSprite* pOther) override;

private:
    ParticleType particleType = DEFAULT;
    Sprite* pTravelTarget = nullptr;
    bool deleteOnReach = false;
    bool targetReached = false;

    float acceleration = 0;

    void (Particle::*updateFunction)(long long elapsedTimeInMilliseconds) = nullptr;

    void initParticle();

    void updateTravel(long long elapsedTimeInMilliseconds);
    void updateDefault(long long elapsedTimeInMilliseconds);

    void randomizeDirection(QVector2D &direction) const;

    void setRandomVelocity();
    void deleteOnFadeEnd();
};


#endif //INC_2023_JCO_AIRTIME_PARTICLE_H
