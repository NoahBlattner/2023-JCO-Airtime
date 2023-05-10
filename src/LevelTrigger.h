/**
\file     LevelTrigger.h
\brief    Déclaration de la classe LevelTrigger.
\author   Blattner Noah
\date     avril 2023
*/

#ifndef INC_2023_JCO_AIRTIME_LEVELTRIGGER_H
#define INC_2023_JCO_AIRTIME_LEVELTRIGGER_H

#include "AdvancedCollisionSprite.h"

class GameCore;

//! A class that can be used to trigger a level change
//!
//! This class is used to trigger a level change.
//! This is useful for creating doors and other objects that trigger a level change.
//!
//! On construction, the LevelTrigger takes a pointer to a GameCore and a QString with the level name.
//!
//! This class is a subclass of AdvancedCollisionSprite,
//! It simply awaits a collision with an Player entity to trigger a level change.
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
