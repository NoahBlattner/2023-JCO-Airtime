//
// Created by blatnoa on 08.05.2023.
//

#ifndef INC_2023_JCO_AIRTIME_LEVELTRIGGER_H
#define INC_2023_JCO_AIRTIME_LEVELTRIGGER_H

#include "AdvancedCollisionSprite.h"

class GameCore;

class LevelTrigger : public AdvancedCollisionSprite {

public :
    LevelTrigger(GameCore* gameCore, QString levelName);

protected:
    virtual void onTrigger(AdvancedCollisionSprite* pOther) override;

private:
    GameCore* m_pCore;
    QString m_levelName;
};


#endif //INC_2023_JCO_AIRTIME_LEVELTRIGGER_H
