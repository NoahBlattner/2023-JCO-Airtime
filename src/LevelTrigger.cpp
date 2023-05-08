//
// Created by blatnoa on 08.05.2023.
//

#include "LevelTrigger.h"

#include "GameCore.h"
#include <QDir>
LevelTrigger::LevelTrigger(GameCore* gameCore, QString levelName) {
    m_pCore = gameCore;
    m_levelName = levelName;

    // Set pixmap for testing
    setPixmap(QPixmap(QDir::toNativeSeparators(GameFramework::imagesPath() + "/kill-zone.png")));

    // Set this to be a trigger
    isTrigger = true;
}

/**
 * Override of the onTrigger method
 * Loads the level when the player collides with the trigger
 * @param pOther The other sprite
 */
void LevelTrigger::onTrigger(AdvancedCollisionSprite* pOther) {
    AdvancedCollisionSprite::onTrigger(pOther);

    // If the other sprite is the player
    if (pOther->collisionTag == "Player") {
        // Load the level specified in the constructor
        m_pCore->loadLevel(m_levelName);
    }
}
