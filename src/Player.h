//
// Created by blatnoa on 22.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_PLAYER_H
#define INC_2023_JCO_AIRTIME_PLAYER_H

#include "sprite.h"

#include <QVector2D>

class GameCore;

class Player : public Sprite {
    Q_OBJECT

public:
    explicit Player(GameCore* gamecore);

    const int PLAYER_SPEED = 1;

    inline QVector2D moveDirection() const { return m_moveDirection; }

    void move(QVector2D moveVector);

    virtual void setParentScene(GameScene* pScene) override;

private:
    QVector2D m_moveDirection = QVector2D(0, 0);

private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);
};


#endif //INC_2023_JCO_AIRTIME_PLAYER_H
