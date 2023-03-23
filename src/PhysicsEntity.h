//
// Created by blatnoa on 22.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_PHYSICSENTITY_H
#define INC_2023_JCO_AIRTIME_PHYSICSENTITY_H

#include "sprite.h"

#include <QVector2D>

class PhysicsEntity : public Sprite {

public:
    PhysicsEntity(QGraphicsItem* pParent = nullptr);
    PhysicsEntity(const QString& rImagePath, QGraphicsItem* pParent = nullptr);

    void setParentScene(GameScene* pScene) override;

    inline QVector2D velocity() const { return m_velocity; }
    inline void addVelocity(QVector2D velocity) { m_velocity += velocity; }
    inline void setVelocity(QVector2D velocity) { m_velocity = velocity; }
    inline void setXVelocity(float xVelocity) { m_velocity.setX(xVelocity); }
    inline void setYVelocity(float yVelocity) { m_velocity.setY(yVelocity); }

    void move(QVector2D moveVector);

    inline bool isGravityEnabled() const { return m_gravityEnabled; }
    inline void setGravityEnabled(bool enabled) { m_gravityEnabled = enabled; }

    inline bool isOnGround() const { return m_isOnGround; }

    bool reevaluateGrounded();

    void tick(long long elapsedTimeInMilliseconds) override;

private:
    const float GRAVITY = 12;
    const float GROUND_DISTANCE = 1;

    QVector2D m_velocity = QVector2D(0, 0);

    bool m_gravityEnabled = true;
    bool m_isOnGround = false;

};


#endif //INC_2023_JCO_AIRTIME_PHYSICSENTITY_H
