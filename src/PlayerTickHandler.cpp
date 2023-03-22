//
// Created by blatnoa on 22.03.2023.
//

#include "PlayerTickHandler.h"

#include "GameCore.h"
#include "Player.h"
#include <QVector2D>

PlayerTickHandler::PlayerTickHandler() = default;

void PlayerTickHandler::init() {
    m_pPlayerSprite = dynamic_cast<Player*>(m_pParentSprite);
}

void PlayerTickHandler::tick(long long int elapsedTimeInMilliseconds) {
    // Calculate the move vector
    QVector2D moveVector = m_pPlayerSprite->moveDirection().normalized();
    moveVector *= m_pPlayerSprite->PLAYER_SPEED * elapsedTimeInMilliseconds;

    // Move the player
    m_pPlayerSprite->move(moveVector);
}