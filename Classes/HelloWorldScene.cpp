/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include <cocos/editor-support/cocostudio/SimpleAudioEngine.h>
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static inline cocos2d::Vec2 getPositionForMenuItem(int itemNum) {
    return cocos2d::Vec2 {0, itemNum * 24.0f};
}

bool HelloWorld::init()
{
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    // Audio
    simpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Sprite frames
    {
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("satyr.plist");
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("satyr_idle.plist");
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("satyr_jump.plist");
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("satyr_attack.plist");
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy_idle.plist");
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy_hurt.plist");
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy_die.plist");
    }

    // Animations
    {
        // Satyr
        {
            auto walkSpriteFrames = getSpriteFrames("Satyr_01_Walking_%03d.png", 18);
            auto walkAnimation = cocos2d::Animation::createWithSpriteFrames(walkSpriteFrames, 1 / 18.0f);

            auto idleSpriteFrames = getSpriteFrames("Satyr_01_Idle_%03d.png", 12);
            auto idleAnimation = cocos2d::Animation::createWithSpriteFrames(idleSpriteFrames, 1 / 12.0f);

            auto jumpSpriteFrames = getSpriteFrames("Satyr_01_Jump Loop_%03d.png", 6);
            auto jumpAnimation = cocos2d::Animation::createWithSpriteFrames(jumpSpriteFrames, 1 / 6.0f);

            auto attackSpriteFrames = getSpriteFrames("Satyr_01_Attacking_%03d.png", 12);
            auto attackAnimation = cocos2d::Animation::createWithSpriteFrames(attackSpriteFrames, 1 / 12.0f);


            cocos2d::AnimationCache::getInstance()->addAnimation(walkAnimation, "Satyr_WALK");
            cocos2d::AnimationCache::getInstance()->addAnimation(idleAnimation, "Satyr_IDLE");
            cocos2d::AnimationCache::getInstance()->addAnimation(jumpAnimation, "Satyr_JUMP");
            cocos2d::AnimationCache::getInstance()->addAnimation(attackAnimation, "Satyr_ATTACK");
        }

        // Enemy
        {
            auto idleSpriteFrames = getSpriteFrames("Satyr_03_Idle_%03d.png", 12);
            auto idleAnimation = cocos2d::Animation::createWithSpriteFrames(idleSpriteFrames, 1 / 12.0f);

            auto hurtSpriteFrames = getSpriteFrames("Satyr_03_Hurt_%03d.png", 12);
            auto hurtAnimation = cocos2d::Animation::createWithSpriteFrames(hurtSpriteFrames, 1 / 12.0f);

            auto dieSpriteFrames = getSpriteFrames("Satyr_03_Dying_%03d.png", 15);
            auto dieAnimation = cocos2d::Animation::createWithSpriteFrames(dieSpriteFrames, 1 / 15.0f);

            cocos2d::AnimationCache::getInstance()->addAnimation(idleAnimation, "Enemy_IDLE");
            cocos2d::AnimationCache::getInstance()->addAnimation(hurtAnimation, "Enemy_HURT");
            cocos2d::AnimationCache::getInstance()->addAnimation(dieAnimation, "Enemy_DIE");
        }
    }

    // Physics
    {
        this->getPhysicsWorld()->setGravity(GRAVITY);
        // this->getPhysicsWorld()->setDebugDrawMask(0xffff);
    }

    // Background
    {
        auto background = cocos2d::Sprite::create("background.jpg");
        auto backgroundSize = background->getContentSize();
        background->setScaleX(visibleSize.width / backgroundSize.width);
        auto backgroundBody = PhysicsBody::createEdgeBox(background->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 1.0f));
        backgroundBody->setDynamic(false);

        background->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.6f));
        background->setPhysicsBody(backgroundBody);
        this->addChild(background, 0, "Fight");
    }

    // Player
    {
        auto mainSprite = cocos2d::Sprite::createWithSpriteFrameName("Satyr_01_Idle_000.png");
        auto idleAnimation = cocos2d::AnimationCache::getInstance()->getAnimation("Satyr_IDLE");
        auto walkAnimation = cocos2d::AnimationCache::getInstance()->getAnimation("Satyr_WALK");
        auto jumpAnimation = cocos2d::AnimationCache::getInstance()->getAnimation("Satyr_JUMP");
        auto attackAnimation = cocos2d::AnimationCache::getInstance()->getAnimation("Satyr_ATTACK");
        auto satyrBody = PhysicsBody::createBox(Size(50.0f , 50.0f ), PhysicsMaterial(0.0f, 0.0f, 1.0f));
        satyrBody->setDynamic(true);
        satyrBody->setRotationEnable(false);

        player = new Player();
        player->initHPBar(origin + cocos2d::Vec2(visibleSize.width / 6, visibleSize.height - visibleSize.height / 8), "LoadingBarFile.png", true);
        player->addHP(100);
        player->setSprite(mainSprite);
        player->setPosition(origin + visibleSize * 0.5f);
        player->addAnimation(PlayerState::IDLE, idleAnimation);
        player->addAnimation(PlayerState::WALK, walkAnimation);
        player->addAnimation(PlayerState::JUMP, jumpAnimation);
        player->addAnimation(PlayerState::ATTACK, attackAnimation);
        player->setPhysicsBody(satyrBody);
        this->addChild(player->getSprite(), 1, "Player");
        this->addChild(player->getHPBar(), 2, "PlayerHPBar");
    }

    // Enemy
    {
        auto mainSprite = cocos2d::Sprite::createWithSpriteFrameName("Satyr_03_Idle_000.png");
        auto deadSprite = cocos2d::Sprite::createWithSpriteFrameName("Satyr_03_Dying_000.png");
        auto idleAnimation = cocos2d::AnimationCache::getInstance()->getAnimation("Enemy_IDLE");
        auto hurtAnimation = cocos2d::AnimationCache::getInstance()->getAnimation("Enemy_HURT");
        auto dieAnimation = cocos2d::AnimationCache::getInstance()->getAnimation("Enemy_DIE");
        auto satyrBody = PhysicsBody::createBox(Size(50.0f , 50.0f ), PhysicsMaterial(0.0f, 0.0f, 1.0f));
        satyrBody->setDynamic(true);

        enemy = new Enemy();
        enemy->initHPBar(origin + cocos2d::Vec2(visibleSize.width - visibleSize.width / 6, visibleSize.height - visibleSize.height / 8), "LoadingBarFile.png", false);
        enemy->addHP(100);
        enemy->setSprite(mainSprite);
        enemy->setDeadSprite(deadSprite);
        enemy->setPosition(Vec2(origin.x + visibleSize.width * 0.7f, origin.y + visibleSize.height * 0.5f));
        enemy->addAnimation(EnemyState::IDLE, idleAnimation);
        enemy->addAnimation(EnemyState::HURT, hurtAnimation);
        enemy->addAnimation(EnemyState::DEAD, dieAnimation);
        enemy->setPhysicsBody(satyrBody);
        this->addChild(enemy->getSprite(), 1, "Enemy");
        this->addChild(enemy->getHPBar(), 2, "EnemyHPBar");
    }

    // Controllers
    initJoystick();
    initButton();

    // Update callback
    {
        schedule(CC_SCHEDULE_SELECTOR(HelloWorld::update), 1 / 144.0f);
    }

    // Touch callbacks
    {
        // NOTE(vlad): Touches now only for spells or something
        auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this](cocos2d::Touch *touch, cocos2d::Event *event) {
            return onTouchBegan(touch, event);
        };
        touchListener->onTouchMoved = [this](cocos2d::Touch *touch, cocos2d::Event *event) {
            onTouchMoved(touch, event);
        };
        touchListener->onTouchEnded = [this](cocos2d::Touch *touch, cocos2d::Event *event) {
            onTouchEnded(touch, event);
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    }

    // Menu
    {
        auto startLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 24);
        auto exitLabel = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 24);

        auto start = MenuItemLabel::create(startLabel);
        start->setPosition(getPositionForMenuItem(1));
        start->setCallback(CC_CALLBACK_1(HelloWorld::menuStartCallback, this));
        auto exit = MenuItemLabel::create(exitLabel);
        exit->setPosition(getPositionForMenuItem(0));
        exit->setCallback(CC_CALLBACK_1(HelloWorld::menuExitCallback, this));

        auto menu = Menu::create();
        menu->addChild(start);
        menu->addChild(exit);

        auto menuBackground = cocos2d::Sprite::create("be7c2fe4a797d602c60dba26f1ebe243.jpg");
        menuBackground->setPosition(origin + visibleSize * 0.5f);
        auto menuBackgroundSize = menuBackground->getContentSize();
        menuBackground->setScaleX(visibleSize.width / menuBackgroundSize.width);
        menuBackground->addChild(menu);

        this->addChild(menuBackground, 3, "Menu");
    }

    return true;
}

void HelloWorld::update(float t) {
    switch (currentGameState) {
        case GameState::MENU:
        {
            if (!simpleAudioEngine->isBackgroundMusicPlaying()) {
                simpleAudioEngine->playBackgroundMusic("menuSound.mp3", true);
            }
        }
        break;
        case GameState::FIGHT:
        {
            auto joystickPosition = leftJoystick->getStickPosition();
            bool isButtonPressed = joystickButton->getValue();

            enemy->onUpdate(joystickPosition, t);
            player->onUpdate(joystickPosition, t);

            if (isButtonPressed) {
                onAttack(t);
            }

            // Update rate is way higher so value is here for 3 - 5 update function runs
            joystickButton->setValue(false);
        }
        break;
    }
}

void HelloWorld::onAttack(float t) {
    if (enemy->getState() == EnemyState::DEAD) {
        // Seems like there is no one to kill
        return;
    }

    if (player->getState() != PlayerState::ATTACK) { // If he is not attacking right now
        player->setState(PlayerState::ATTACK);
        float distance = player->getPosition().distance(enemy->getPosition());
        if (distance <= 80.0f) {
            // So we striked an enemy
            enemy->addHP(-20);
            enemy->setState(EnemyState::HURT);
        }
    }
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    return true;
}
void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
}

cocos2d::Vector<cocos2d::SpriteFrame *> HelloWorld::getSpriteFrames(const char *fmt, int count) {
    auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
    cocos2d::Vector<cocos2d::SpriteFrame *> spriteFrames;
    char str[256];
    for (int i = 0; i < count; ++i) {
        sprintf(str, fmt, i);
        spriteFrames.pushBack(spriteFrameCache->getSpriteFrameByName(str));
    }
    return spriteFrames;
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
}

void HelloWorld::initJoystick() {
    auto screenSize = Director::getInstance()->getVisibleSize();
    auto screenOrigin = Director::getInstance()->getVisibleOrigin();

    Rect joystickBaseDimensions;
    joystickBaseDimensions = Rect(0, 0, 100.f, 100.0f);

    Point joystickBasePosition;
    joystickBasePosition = Vec2(screenOrigin.x + screenSize.width / 8.0f, screenOrigin.y + screenSize.height / 6.0f);

    SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
    joystickBase->init();
    joystickBase->setPosition(joystickBasePosition);
    joystickBase->setBackgroundSprite(Sprite::create("joystickBackgroud.png"));
    joystickBase->setThumbSprite(Sprite::create("thumb.png"));

    SneakyJoystick *aJoystick = new SneakyJoystick();
    aJoystick->initWithRect(joystickBaseDimensions);
    aJoystick->autorelease();
    joystickBase->setJoystick(aJoystick);
    joystickBase->setPosition(joystickBasePosition);

    leftJoystick = joystickBase->getJoystick();
    leftJoystick->retain();
    this->addChild(joystickBase, 1, "LeftJoystick");
}

void HelloWorld::initButton() {
    auto screenSize = Director::getInstance()->getVisibleSize();
    auto screenOrigin = Director::getInstance()->getVisibleOrigin();

    Rect attackButtonDimensions = Rect(0, 0, 64.0f, 64.0f);
    Point attackButtonPosition;
    attackButtonPosition = Point(screenOrigin.x + screenSize.width * 0.9f, screenOrigin.y + screenSize.height * 0.2f);

    SneakyButtonSkinnedBase *attackButtonBase = SneakyButtonSkinnedBase::create();
    attackButtonBase->setPosition(attackButtonPosition);

    attackButtonBase->setDefaultSprite(Sprite::create("button.png"));
    attackButtonBase->setActivatedSprite(Sprite::create("button.png"));
    attackButtonBase->setDisabledSprite(Sprite::create("buttonBlur.png"));
    attackButtonBase->setPressSprite(Sprite::create("buttonBlur.png"));

    SneakyButton *aAttackButton = SneakyButton::create();
    aAttackButton->initWithRect(attackButtonDimensions);

//  aAttackButton->setIsHoldable(true);
//  aAttackButton->setIsToggleable(true);

    attackButtonBase->setButton(aAttackButton);
    attackButtonBase->setPosition(attackButtonPosition);

    joystickButton = attackButtonBase->getButton();
    joystickButton->retain();
    this->addChild(attackButtonBase, 1, "RightJoystickButton");
}

void HelloWorld::menuStartCallback(cocos2d::Ref *sender) {
    currentGameState = GameState::FIGHT;

    simpleAudioEngine->stopBackgroundMusic(false);

    this->getChildByName("Menu")->setVisible(false);
}

void HelloWorld::menuExitCallback(cocos2d::Ref *sender) {
    Director::getInstance()->end();
}