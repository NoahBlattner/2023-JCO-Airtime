//
// Created by blatnoa on 27.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_ADVANCEDCOLLISIONSPRITE_H
#define INC_2023_JCO_AIRTIME_ADVANCEDCOLLISIONSPRITE_H


#include "sprite.h"

class AdvancedCollisionSprite : public Sprite {

    Q_OBJECT

public:
    explicit AdvancedCollisionSprite(QGraphicsItem* pParent = nullptr);
    AdvancedCollisionSprite(const QString& rImagePath, QGraphicsItem* pParent = nullptr);

protected:
    bool isTrigger = false;
    inline void setTrigger(bool trigger) { isTrigger = trigger;};
    [[nodiscard]] inline bool getTrigger() const { return isTrigger; };

    // List of colliding classes
    QList<QString> collidingClasses = {"Sprite"};
    inline void addCollidingClass(const QString& className) { collidingClasses.append(className); };
    inline void removeCollidingClass(const QString& className) { collidingClasses.removeOne(className); };

    void alignRectToSprite(QRectF newRect, const Sprite* sprite) const;

    void onTrigger(Sprite* pOther);
    virtual void onCollision(Sprite* pOther);

    void checkIntersects();
    void checkIntersects(QRectF rect);

signals:
    void notifyTrigger(Sprite* pOther);
    void notifyCollision(Sprite* pOther);
};


#endif //INC_2023_JCO_AIRTIME_ADVANCEDCOLLISIONSPRITE_H
