//
// Created by blatnoa on 27.03.2023.
//

#include "DirectionalEntityCollision.h"

DirectionalEntityCollision::DirectionalEntityCollision(QGraphicsItem* pParent) : AdvancedCollisionSprite(pParent) {}
DirectionalEntityCollision::DirectionalEntityCollision(const QString& rImagePath, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {}
DirectionalEntityCollision::DirectionalEntityCollision(const QString& rImagePath, Directions directions, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {
    m_directions = directions;
}