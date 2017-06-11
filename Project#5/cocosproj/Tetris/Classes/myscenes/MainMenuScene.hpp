#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "../HelloWorldScene.h"
//#include "SceneManagement.hpp"
#include <iostream>
using namespace std;
using namespace cocos2d;
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif
//using namespace Tetris::Views;
namespace Tetris{
    namespace Cocos2dScenes{
class MainMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(){
       MainMenuScene* scene = MainMenuScene::create();
        return scene;
    }
    virtual void onEnter(){
        Scene::onEnter();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
                                                                              "res/bgms/mainmenu_bgm.mp3", true);
    }
    virtual void onExit(){
        Scene::onExit();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
    virtual bool init(){
         if ( !Scene::init() )
    {
        return false;
    }
        auto img = Sprite::create("res/backgrounds/mainbg.jpg");
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
                                                                              "res/bgms/mainmenu_bgm.mp3", true);
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Size spritesz = img->getContentSize();
        img->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
        img->setScale(visibleSize.width/spritesz.width, visibleSize.height/spritesz.height);
        this->addChild(img);
        auto menu =this->generateOptionMenu();
        this->addChild(menu,1);
        return true;
    }
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender){
        Director::getInstance()->end();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
        #endif
    }
    void menuPlayBtnCallback(cocos2d::Ref* pSender){
        auto scene =  HelloWorld::createScene();
        Director::getInstance()->pushScene(scene);
        //Director::getInstance()->replaceScene(scene);
    }
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
    protected:
        cocos2d::Menu* generateOptionMenu(){
            auto item_1 = MenuItemFont::create("New Game", CC_CALLBACK_1(MainMenuScene::menuPlayBtnCallback, this));
            auto item_3 = MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
            auto menu = Menu::create(item_1,item_3,NULL);
           // menu->setPosition(Vec2::ZERO);
            menu->alignItemsVertically();
            return menu;
        }
};
}}
#endif // __MAINMENU_SCENE_H__
