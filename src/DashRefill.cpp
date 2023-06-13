//
// Created by blatnoa on 15.05.2023.
//

#include <QDir>
#include "resources.h"
#include "Player.h"

#include "DashRefill.h"

const int RESPAWN_TIME = 2500;

//! Constructor :
//! Automatically sets the image of the collectible.
//! \param pParent The parent of the collectible.
DashRefill::DashRefill(QGraphicsItem* pParent) : Collectible(RESPAWN_TIME, pParent) {
    setPixmap(QPixmap(QDir::toNativeSeparators(GameFramework::imagesPath() + "energy.png")));

    setSound(QDir::toNativeSeparators(GameFramework::soundsPath() + "dash-refill.wav"));
}

//! Override of the onCollect function from Collectible.
//! Recharges the player's dash.
//! \param player The player that collected the collectible.
void DashRefill::onCollect(Player* player) {
    if (player->canDash()) {
        // Player already has full dash
        return;
    }
    Collectible::onCollect(player);

    player->rechargeDash();
}
