//
// Created by Vlad on 25.08.2020.
//

#ifndef PROJ_ANDROID_PLAYER_H
#define PROJ_ANDROID_PLAYER_H

#include "cocos2d.h"
#include "Character.h"
#include "Enemy.h"

const float PLAYER_WALK_SPEED = 150.0f;
const float PLAYER_JUMP_SPEED = 300.0f;

class PlayerState : public CharacterState {
public:
    // ...
};

class Player : public Character {
public:
    void onUpdate(const cocos2d::Vec2 &joystickPosition, float t);
    ~Player() override;
private:
};


#endif //PROJ_ANDROID_PLAYER_H
