//
// Created by blatnoa on 15.05.2023.
//

#include "gamescene.h"

#include "Collectible.h"
#include "Particle.h"
#include "resources.h"
#include "Player.h"

//! Constructor :
//! Automatically sets the collectible to be a trigger.
//! \param pParent The parent of the collectible.
Collectible::Collectible(unsigned int respawnTime, QGraphicsItem* pParent) : Collectible("", respawnTime, pParent) {
}

//! Constructor :
//! Automatically sets the collectible to be a trigger.
//! \param rImagePath The path to the image of the collectible.
//! \param pParent The parent of the collectible.
Collectible::Collectible(const QString &rImagePath, unsigned int respawnTime, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, pParent) {
    isTrigger = true;
    m_respawnTime = respawnTime;
}

//! Constructor :
//! Automatically sets the collectible to be a trigger.
//! \param rImagePath The path to the image of the collectible.
//! \param collisionOverride The collision of the collectible.
//! \param pParent The parent of the collectible.
Collectible::Collectible(const QString &rImagePath, QRectF collisionOverride, unsigned int respawnTime, QGraphicsItem* pParent) : AdvancedCollisionSprite(rImagePath, collisionOverride, pParent) {
    isTrigger = true;
    m_respawnTime = respawnTime;
}

//! Override of the onTrigger function from AdvancedCollisionSprite:
//! When the collectible is intersecting with a player, the onCollect function is called.
void Collectible::onTrigger(AdvancedCollisionSprite* pOther) {
    AdvancedCollisionSprite::onTrigger(pOther);

    if (pOther->collisionTag == "Player" && isEnabled()) {
        // Collectible was collected by player
        onCollect((Player*) pOther);
    }
}

//! Called when the collectible is collected by a player.
//! Disables the collectible
//! \param player The player that collected the collectible.
void Collectible::onCollect(Player* player) {
    disable();
    spawnCollectParticles(player);

    if (m_collectSound.isLoaded()) {
        // Play the collect sound
        m_collectSound.play();
    }
}

//! Disables the collectible and hides it.
//! If the respawn time is set to 0, the collectible will be destroyed.
//! Else, the collectible will reappear after the specified amount of time.
void Collectible::disable() {
    // Collectible is rendered invisible and disabled
    setVisible(false);
    setEnabled(false);

    if (m_respawnTime == 0) {
        // Destroy the collectible
        delete this;
    } else {
        // Collectible is disabled and will reappear after a delay
        QTimer::singleShot(5000, this, &Collectible::enable);
    }
}

//! Enables the collectible by reactivating it's collisions and rendering it visible.
void Collectible::enable() {
    // Collectible is rendered visible and enabled
    setVisible(true);
    setEnabled(true);
}

//! Spawn particles that travel towards the player.
void Collectible::spawnCollectParticles(Player* pPlayer, int particleCount) {
    for (int i = 1; i < particleCount; i++) {
        auto* particle = new Particle(Particle::TRAVEL, GameFramework::imagesPath() + "particle.png");
        particle->setPos(sceneBoundingRect().center());
        particle->setScale(.1);
        particle->setTravelTarget(pPlayer);
        particle->initialSpeed = 7.5;
        particle->setAcceleration(0.925f);
        particle->fadeTime = .5f;
        parentScene()->addSpriteToScene(particle);
    }
}

//! Sets the sound that is played when the collectible is collected.
//! \param rSoundPath The path to the sound file.
void Collectible::setSound(const QString &rSoundPath) {
    m_collectSound.setSource(QUrl::fromLocalFile(rSoundPath));
}
