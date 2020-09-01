//
// Created by Vlad on 29.08.2020.
//

#ifndef PROJ_ANDROID_ENEMY_H
#define PROJ_ANDROID_ENEMY_H

#include "cocos2d.h"
#include "Character.h"

class EnemyState : public CharacterState {
public:
    // ...
};

class Enemy : public Character {
public:
    ~Enemy() override;
    void onUpdate(const cocos2d::Vec2 &joystickPosition, float t);
};


#endif //PROJ_ANDROID_ENEMY_H
