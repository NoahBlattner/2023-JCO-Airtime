//
// Created by blatnoa on 22.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_PHYSICSENTITY_H
#define INC_2023_JCO_AIRTIME_PHYSICSENTITY_H

#include "AdvancedCollisionSprite.h"

#include <QVector2D>

//! A sprite that is affected by physics
//! This includes gravity and friction
//!
//! This class can be subclassed to create various physics entities such as the player or enemies
//!
//! This class is always automatically registered for ticks when the parent scene is set
//!
//! The physics entity has a velocity vector that is applied to the entity every tick
//! The velocity is affected by gravity and friction
//! Gravity is applied by default, but can be disabled
//!
//! On every tick, the entity is moved according to it's velocity vector
//! Before moving, the entity checks for collisions with other sprites
//! If a collision is detected, the entity is moved to the closest position to the collision point
//! Collision detection is done using the parent AdvancedCollisionSprite class
//!
//! The isOnGround property is set to true if the entity is on the ground
//! The entity is considered to be on the ground if the distance to the ground is less than GROUNDED_DISTANCE constant
//! If needed, the isGrounded property can be reevaluated by calling the reevaluateGrounded function at any time
//! This is normally not needed, as the isGrounded property is automatically reevaluated every time the entity moves
//!
//! The class always registers for collision events with the DirectionalEntityCollider class
//! When a collision with a DirectionalEntityCollider is detected, the entity checks if the collisions is blocking the current direction of movement
class PhysicsEntity : public AdvancedCollisionSprite {

    Q_OBJECT

public:
    explicit PhysicsEntity(QGraphicsItem* pParent = nullptr);
    explicit PhysicsEntity(const QString& rImagePath, QGraphicsItem* pParent = nullptr);

    void setParentScene(GameScene* pScene) override;

    // Velocity
    [[nodiscard]] inline QVector2D velocity() const { return velocityVector; }
    inline void addVelocity(QVector2D velocity) { velocityVector += velocity; }
    inline void setVelocity(QVector2D velocity) { velocityVector = velocity; }
    inline void setXVelocity(float xVelocity) { velocityVector.setX(xVelocity); }
    inline void setYVelocity(float yVelocity) { velocityVector.setY(yVelocity); }

    void move(QVector2D moveVector);

    // Gravity
    [[nodiscard]] inline bool isGravityEnabled() const { return gravityEnabled; }
    inline void setGravityEnabled(bool enabled) { gravityEnabled = enabled; }

    // Ground check
    [[nodiscard]] inline bool isOnGround() const { return m_isOnGround; }
    bool reevaluateGrounded();

    void tick(long long elapsedTimeInMilliseconds) override;

private:
    const float GROUNDED_DISTANCE = 1;
    bool m_isOnGround = false;

    QRectF m_newRect;

protected:
    QVector2D velocityVector = QVector2D(0, 0);

    float gravity = -9.81;
    bool gravityEnabled = true;

    float friction = .15;

    void limitRectToScene(QRectF &rect) const;

    void alignRectToSprite(QRectF &rect, const Sprite* pSprite);

    void onCollision(Sprite* pOther) override;
};


#endif //INC_2023_JCO_AIRTIME_PHYSICSENTITY_H
