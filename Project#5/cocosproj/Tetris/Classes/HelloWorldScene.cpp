#include <iostream>
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "tet2/GameController.hpp"
USING_NS_CC;
using namespace cocos2d;
using namespace std;
using namespace Tetris;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

void HelloWorld::makeField(){
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    const int hei = (int)gc->getGameHeight();
    const int wid = (int)gc->getGameWidth();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    const int echhei = (int)((0.9f*(float)visibleSize.height)/hei);
    const int echwid =  (int)((0.9f*(float)visibleSize.width)/wid);
    for (int i = 0; i < hei; i++) {
        for (int j = 0; j < wid; j++) {
            auto* b = Label::createWithTTF("□", "fonts/arial.ttf", 24.0);
            b->setPosition(Vec2(origin.x + echwid*(j+1),
                             echhei*i - b->getContentSize().height));
            //b->setPosition(ccp(winSize.width * (0.32 + j * 0.04), winSize.height * (0.1 + i * 0.04)));
            //b->setColor(ccc3(128, 128, 128,255));
            b->setColor(ccc3(128, 128, 128));
            this->addChild(b);
        }
    }
}
void HelloWorld::startGame(){
    //this->scheduleOnce(schedule_selector(HelloWorld::play), 0);
}
void HelloWorld::gameloop(){
   // HelloWorld::gc->innergameloop();
}
void HelloWorld::play(){
   // HelloWorld::gc->justinit();
   //HelloWorld::gc->setGameStatusToOngoing();
   // this->schedule(schedule_selector(HelloWorld::gameloop), 1);
    /*while(!this->gc->forceend&&!this->isEnd()){
                    while(!this->forceend&&this->gs!=GameStatus::ONGOING){
                        cout<<"new loop"<<endl;
                        //this->printposinfo();

                        this->printcurrentboard();
                        cout<<"printboard ok"<<endl;
                        if(checkEnd()){this->gs = GameStatus::END;}
                        cout<<"check end ok"<<endl;
                        if(!this->usercheck())break;
                        Users::GameUser* guser = this->gusers[0];
                        this->findAndRemoveLines();
                        cout<<"rm ln ok"<<endl;
                        if(this->candropdown()){
                            guser->setCurrentY(guser->getCurrentY()-1);
                            cout<<"drop if inner ok"<<endl;
                        }else{
                            this->saveBlockAndCheck(guser);
                            cout<<"save block ok"<<endl;
                        }
                        cout<<"drop down ok"<<endl;
                        gplaytime++;
                        usleep(1000000);
                        cout<<"1 loop end"<<endl;
                    }
                    if(this->gs!=GameStatus::END){
                        usleep(1000000/2);
                    }
                }*/
}
void HelloWorld::pause(){
    HelloWorld::gc -> pause();
}
void HelloWorld::resume(){
    HelloWorld::gc->resume();
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    if(HelloWorld::gc==NULL){
        HelloWorld::gc = new GameController();
        cout<<(int)gc->getGameHeight()<<endl;
    }
    this->makeField();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    cout<<"loaded ok"<<endl;
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
