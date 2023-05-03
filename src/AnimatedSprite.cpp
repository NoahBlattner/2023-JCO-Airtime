//
// Created by blatnoa on 03.05.2023.
//

#include "AnimatedSprite.h"

#include <utility>

AnimatedSprite::AnimatedSprite(const QImage& animation, QList<int> frameDurations, bool loop) : Sprite(){
    createAnimation(animation, std::move(frameDurations));

    if (!loop) { // If the animation is not supposed to loop
        // Connect the animationFinished signal to the deleteLater slot
        setEmitSignalEndOfAnimationEnabled(true);
        connect(this, &AnimatedSprite::animationFinished, this, &AnimatedSprite::deleteLater);
    }

    startAnimation();
}
