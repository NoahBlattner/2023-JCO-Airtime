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
    explicit AdvancedCollisionSprite(const QString& rImagePath, QGraphicsItem* pParent = nullptr);

    // Trigger
    inline void setTrigger(bool trigger) { isTrigger = trigger;};
    [[nodiscard]] inline bool getIsTrigger() const { return isTrigger; };

    // Colliding classes
    void addCollidingClass(const QString& className);
    inline void removeCollidingClass(const QString& className) { collidingClasses.removeOne(className); };
    void collideAll();
    QList<Sprite*> getCollidingSprites(QRectF rect = QRectF());

    // Intersection events
    void onTrigger(Sprite* pOther);
    virtual void onCollision(Sprite* pOther);

    // Intersections checks
    virtual void reevaluateIntersects();
    virtual void reevaluateIntersects(QRectF rect);

protected:
    // Trigger
    bool isTrigger = false;

    // Colliding classes
    QList<QString> collidingClasses = {"All"};

signals:
    void notifyTrigger(Sprite* pOther);
    void notifyCollision(Sprite* pOther);
};


#endif //INC_2023_JCO_AIRTIME_ADVANCEDCOLLISIONSPRITE_H
