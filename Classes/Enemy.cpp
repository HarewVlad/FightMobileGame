//
// Created by Vlad on 29.08.2020.
//

#include "Enemy.h"

Enemy::~Enemy() {

}

void Enemy::onUpdate(const cocos2d::Vec2 &joystickPosition, float t) {
    if (currentState == EnemyState::DEAD) {
        // WE DON'T EXISTS
        return;
    }

    if (hp == 0) {
        // We just died
        // setState(EnemyState::DEAD);
        currentState = EnemyState::DEAD;

        // TODO: make it work or redesign the code around setState and playAnimation ...
        sprite->stopAllActions();
        auto animation = getAnimation(EnemyState::DEAD);
        auto animate = cocos2d::Animate::create(animation);
        auto fadeOut = cocos2d::FadeOut::create(1.0f);
        auto callbackDeath = cocos2d::CallFunc::create([this]() {
            sprite->removeFromParentAndCleanup(true);
        });
        auto seq = cocos2d::Sequence::create(animate, fadeOut, callbackDeath, nullptr);
        sprite->runAction(seq);
    }
}