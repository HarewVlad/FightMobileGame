//
// Created by Vlad on 29.08.2020.
//

#ifndef PROJ_ANDROID_CHARACTER_H
#define PROJ_ANDROID_CHARACTER_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class CharacterState {
public:
    static const int IDLE = 0;
    static const int WALK = 1;
    static const int JUMP = 2;
    static const int HURT = 3;
    static const int ATTACK = 4;
    static const int DEAD = 5;
};

class Character {
public:
    virtual void initHPBar(const cocos2d::Vec2 &position, const std::string &filename, bool toRight) {
        hpBar = cocos2d::ui::LoadingBar::create(filename);
        hpBar->setPosition(position);
        hpBar->setScale(2);

        if (toRight) {
            hpBar->setDirection(cocos2d::ui::LoadingBar::Direction::RIGHT);
        } else {
            hpBar->setDirection(cocos2d::ui::LoadingBar::Direction::LEFT);
        }
    }

    virtual inline void addAnimation(int name, cocos2d::Animation *animation) noexcept {
        animations[name] = animation;
    }

    virtual inline void playAnimation(int state, float delayTime = 0.0f, bool isInfinite = true) {
        if (currentAnimation != state) {
            sprite->stopAllActions();
            auto animation = getAnimation(state);
            auto animate = cocos2d::Animate::create(animation);
            if (isInfinite) {
                sprite->runAction(cocos2d::RepeatForever::create(animate));
            } else {
                auto delay = cocos2d::DelayTime::create(delayTime);
                auto callbackAnimate = cocos2d::CallFunc::create([this]() {
                   setState(CharacterState::IDLE);
                });
                auto seq = cocos2d::Sequence::create(delay, animate, callbackAnimate, nullptr);
                sprite->runAction(seq);
            }

            currentAnimation = state;
        }
    }

    // Setters
    virtual inline void setSprite(cocos2d::Sprite *sprite) noexcept {
        this->sprite = sprite;
    }
    virtual inline void setDeadSprite(cocos2d::Sprite *sprite) noexcept {
        this->deadSprite = sprite;
    }
    virtual inline void setPosition(const cocos2d::Vec2 &position) {
        if (sprite != nullptr) {
            sprite->setPosition(position);
        } else {
            throw std::runtime_error("Sprite is nullptr");
        }
    }
    virtual inline void setPhysicsBody(cocos2d::PhysicsBody *body) {
        if (sprite != nullptr) {
            sprite->setPhysicsBody(body);
        } else {
            throw std::runtime_error("Sprite is nullptr");
        }
    }
    virtual inline void setState(int state, float delayTime = 0) { // NOTE: if need more, redefine method in derived class
        if (currentState != state) {
            currentState = state;
            switch (state) {
                case CharacterState::IDLE:
                    playAnimation(CharacterState::IDLE);
                    break;
                case CharacterState::WALK:
                    playAnimation(CharacterState::WALK);
                    break;
                case CharacterState::JUMP:
                    playAnimation(CharacterState::JUMP);
                    break;
                case CharacterState::HURT:
                    playAnimation(CharacterState::HURT, delayTime,false);
                    break;
                case CharacterState::ATTACK:
                    playAnimation(CharacterState::ATTACK, delayTime,false);
                    break;
                case CharacterState::DEAD:
                    playAnimation(CharacterState::DEAD, delayTime, false);
                    break;
                default:
                    throw std::runtime_error("State with ID = " + std::to_string(state) + " not defined");
            }
        }
    }

    // Getters
    virtual inline int getState() const noexcept {
        return currentState;
    }
    virtual inline cocos2d::Sprite *getSprite() const noexcept {
        return sprite;
    }
    virtual inline cocos2d::Sprite *getDeadSprite() const noexcept {
        return deadSprite;
    }
    virtual inline cocos2d::Animation *getAnimation(int animation) const {
        return animations.at(animation);
    }
    virtual inline cocos2d::Vec2 getPosition() const {
        return sprite->getPosition();
    }
    virtual inline cocos2d::ui::LoadingBar *getHPBar() const noexcept {
        return hpBar;
    }
    virtual inline void addHP(int value) noexcept {
        hp += value;
        hpBar->setPercent(hp);
    }
    virtual inline float getHP() const noexcept {
        return hp;
    }

    virtual ~Character() {}
public:
    cocos2d::Sprite *sprite;
    cocos2d::Sprite *deadSprite;
    std::unordered_map<int, cocos2d::Animation *> animations;
    cocos2d::ui::LoadingBar *hpBar;
    int hp;
    int currentState;
    int currentAnimation;
};

#endif //PROJ_ANDROID_CHARACTER_H
