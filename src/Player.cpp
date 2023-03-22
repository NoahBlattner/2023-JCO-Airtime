//
// Created by blatnoa on 22.03.2023.
//

#include "Player.h"

#include "resources.h"
#include <QDir>

Player::Player() {
    setPixmap(QPixmap(QDir::toNativeSeparators(GameFramework::resourcesPath() + "/images/player.png")));
}
