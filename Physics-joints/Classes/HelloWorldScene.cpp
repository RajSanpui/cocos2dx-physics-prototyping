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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "../cocos2d/cocos/deprecated/CCDeprecated.h"

#define FLOOR_HEIGHT 62.0f

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
    //return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    //if ( !Scene::init() )
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label



    //Set up sprite
    CCSprite *sprite = CCSprite::create("bg.png");
    //sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
    addChild(sprite,-1);

    catapult = CCSprite::create("catapult.png");
    catapult->setAnchorPoint(CCPointZero);
    catapult->setPosition(CCPointMake(181.0f,FLOOR_HEIGHT));

    initializePhysics(catapult);
    catapult->getPhysicsBody()->setDynamic(false);

    chain = CCSprite::create("chain.png");
    chain->setAnchorPoint(CCPointZero);
    chain->setPosition(CCPointMake(181.0f,FLOOR_HEIGHT+45));

    initializePhysics(chain);
    //chain->getPhysicsBody()->setDynamic(false);

    auto offset=Vec2(10.0, 10.0);
    //PhysicsJointFixed* joint = PhysicsJointFixed::construct(catapult->getPhysicsBody(), chain->getPhysicsBody(), offset);
    addChild(chain, 0);
    addChild(catapult, 0);


    initTouch();
    return true;
}

void HelloWorld::initializePhysics(Sprite* sprite)
{
    auto circle = PhysicsBody::createCircle(sprite -> getContentSize().width / 2);
    circle->setContactTestBitmask(true);
    //circle->setDynamic(true);
    sprite->setPhysicsBody(circle);
}

//https://stackoverflow.com/questions/32021279/cocos2d-x-3-6-physicsjointfixed-error
void HelloWorld::onEnterTransitionDidFinish() {

    PhysicsJointSpring* joint = PhysicsJointSpring::construct(catapult->getPhysicsBody(), chain->getPhysicsBody(), Point::ZERO, Point::ZERO, 10.0f, 10.0f);
    //joint = PhysicsJointFixed::construct(_square->getPhysicsBody(), _square1->getPhysicsBody(),Vec2(100,100));
    this->getScene()->getPhysicsWorld()->addJoint(joint);
}

void HelloWorld::initTouch()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener -> onTouchBegan = [] (Touch* touch, Event* event) { return true;};
    listener -> onTouchMoved = CC_CALLBACK_2(HelloWorld::moveSprite, this);
    listener -> onTouchEnded = [=] (Touch* touch, Event* event) {};
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::moveSprite(Touch* touch, Event* evento)
{
    Vec2 touchLocation = touch->getLocation();
    Vec2 offset = touchLocation - chain->getPosition();

    Vec2 currPosition = touch->getLocation();

    if(chain->getBoundingBox().containsPoint(currPosition)){
        chain-> setPositionX(currPosition.x);
    }
    /* // Moving sprite towards x-axis
    Vec2 currPosition = touch->getLocation();
    if(sprite3->getBoundingBox().containsPoint(currPosition)){
        sprite3-> setPositionX(currPosition.x);
    }*/

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
