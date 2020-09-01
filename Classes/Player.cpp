//
// Created by Vlad on 25.08.2020.
//

#include "Player.h"

Player::~Player() {

}

void Player::onUpdate(const cocos2d::Vec2 &joystickPosition, float t) {
    // Movement
    if (!joystickPosition.isZero()) {
        if (joystickPosition.y < 0.5f && joystickPosition.x > 0) { // Right
            auto moveRight = cocos2d::MoveBy::create(t, cocos2d::Vec2(PLAYER_WALK_SPEED * t, 0));
            sprite->setFlippedX(false);
            sprite->runAction(moveRight);

            if (currentState != PlayerState::ATTACK)
                setState(PlayerState::WALK);
        } else if (joystickPosition.y < 0.5f && joystickPosition.x < 0) { // Left
            auto moveLeft = cocos2d::MoveBy::create(t, cocos2d::Vec2(-PLAYER_WALK_SPEED * t, 0));
            sprite->setFlippedX(true);
            sprite->runAction(moveLeft);

            if (currentState != PlayerState::ATTACK)
                setState(PlayerState::WALK);
        } else if (joystickPosition.y > 0.5f && joystickPosition.x > 0.8f) { // Jump right
            auto moveRight = cocos2d::MoveBy::create(t, cocos2d::Vec2(PLAYER_WALK_SPEED * t, 0));
            auto jump = cocos2d::MoveBy::create(t, cocos2d::Vec2(0, PLAYER_JUMP_SPEED * t));
            auto seq = cocos2d::Sequence::create(moveRight, jump, nullptr);
            sprite->setFlippedX(false);
            sprite->runAction(seq);

            if (currentState != PlayerState::ATTACK)
                setState(PlayerState::JUMP);
        } else if (joystickPosition.y > 0.5f && joystickPosition.x < -0.8f) { // Jump left
            auto moveLeft = cocos2d::MoveBy::create(t, cocos2d::Vec2(-PLAYER_WALK_SPEED * t, 0));
            auto jump = cocos2d::MoveBy::create(t, cocos2d::Vec2(0, PLAYER_JUMP_SPEED * t));
            auto seq = cocos2d::Sequence::create(moveLeft, jump, nullptr);
            sprite->setFlippedX(true);
            sprite->runAction(seq);

            if (currentState != PlayerState::ATTACK)
                setState(PlayerState::JUMP);
        }
    } else {
        if (currentState != PlayerState::ATTACK)
            setState(PlayerState::IDLE);
    }
}