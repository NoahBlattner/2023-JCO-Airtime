//
// Created by blatnoa on 22.03.2023.
//

#ifndef INC_2023_JCO_AIRTIME_PLAYERTICKHANDLER_H
#define INC_2023_JCO_AIRTIME_PLAYERTICKHANDLER_H

#include <QObject>
#include "SpriteTickHandler.h"

class GameCore;
class QKeyEvent;
class Player;

class PlayerTickHandler : public QObject, public SpriteTickHandler{
    Q_OBJECT

public:
    explicit PlayerTickHandler();

    void init() override;
    void tick(long long elapsedTimeInMilliseconds) override;

private:
    Player* m_pPlayerSprite;

};


#endif //INC_2023_JCO_AIRTIME_PLAYERTICKHANDLER_H
