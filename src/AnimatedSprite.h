/**
\file     AnimatedSprite.h
\brief    Déclaration de la classe AnimatedSprite.
\author   Blattner Noah
\date     avril 2023
*/

#ifndef INC_2023_JCO_AIRTIME_ANIMATEDSPRITE_H
#define INC_2023_JCO_AIRTIME_ANIMATEDSPRITE_H

class QImage;

#include <QList>
#include "sprite.h"

//! \brief A Sprite whose only purpose is to display an animation.
//!
//! The AnimatedSprite class is a subclass of the Sprite class.
//! It is used to display an animation.
//!
//! A is passed to the AnimatedSprite on construction.
//! The AnimatedSprite then displays the animation.
//!
//! If indicated, the AnimatedSprite can loop the animation.
//! Else, the AnimatedSprite will be destroyed when the animation is finished.
class AnimatedSprite : public Sprite {

public:
    AnimatedSprite(const QImage& animationSpriteSheet, QList<int> frameDurations, bool loop = false);
};


#endif //INC_2023_JCO_AIRTIME_ANIMATEDSPRITE_H
