//
// Created by blatnoa on 03.05.2023.
//

#include "AnimatedSprite.h"

#include <utility>

//! Constructor :
//! Creates an animation from the given sprite sheet and frame durations.
//! If indicated, the animation will loop.
//! Else, the animation will play once and the sprite will be destroyed.
//! \param animationSpriteSheet The sprite sheet to use for the animation.
//! \param frameDurations The durations of each frame of the animation.
//! \param loop Whether the animation should loop or not.
AnimatedSprite::AnimatedSprite(const QImage& animationSpriteSheet, QList<int> frameDurations, bool loop) : Sprite(){
    createAnimation(animationSpriteSheet, std::move(frameDurations));

    if (!loop) { // If the animation is not supposed to loop
        // Connect the animationFinished signal to the deleteLater slot
        setEmitSignalEndOfAnimationEnabled(true);
        connect(this, &AnimatedSprite::animationFinished, this, &AnimatedSprite::deleteLater);
    }

    startAnimation();
}
