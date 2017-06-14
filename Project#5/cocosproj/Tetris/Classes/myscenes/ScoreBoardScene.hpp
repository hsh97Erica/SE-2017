/**
 @file ScoreBoardScene.hpp
 @brief
 */
#ifndef _SCR_BRD_SCENE_H_INC_
#define _SCR_BRD_SCENE_H_INC_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include <queue>
#include <ctime>
#include <string>
#include "../tet2/BaseDBManagement.hpp"
#include "../tet2/DBManagement.hpp"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace Tetris::DBManagement;
using namespace std;
namespace Tetris{
    namespace Cocos2dScenes{
        /**
         @class ScoreBoardScene
         @brief 이전에 플레이한 테트리스의 기록을 보여주는 창의 클래스
         */
        class ScoreBoardScene : public cocos2d::Scene{
            public:
                static cocos2d::Scene* createScene(){
                    ScoreBoardScene* scene = ScoreBoardScene::create();
                    return scene;
                }
            virtual void onEnter(){
                Scene::onEnter();
                CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("res/bgms/scorerecordbgm.mp3", true);
                if(!readSoundEnable()){
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
                }
            }
            virtual void onExit(){
                Scene::onExit();
                CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            }
            
                virtual bool init(){
                    if ( !Scene::init() ){
                        return false;
                    }
                    
                    if(!ScoreDBManager::getInstance()->isOpened()){
                        char* fileptr =(char*) (ScoreDBManager::getDBLocationForTetrisGame()) .c_str();
                        ScoreDBManager::getInstance()->open(fileptr);
                    }
                    auto visibleSize = Director::getInstance()->getVisibleSize();
                    Vec2 origin = Director::getInstance()->getVisibleOrigin();
                    
                    /////////////////////////////
                    // 2. add a menu item with "X" image, which is clicked to quit the program
                    //    you may modify it.
                    
                    // add a "close" icon to exit the progress. it's an autorelease object
                    auto closeItem = MenuItemImage::create(
                                                           "CloseNormal.png",
                                                           "CloseSelected.png",
                                                           CC_CALLBACK_1(ScoreBoardScene::menuCloseCallback, this));
                    
                    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                                origin.y + closeItem->getContentSize().height/2));
                    
                    // create menu, it's an autorelease object
                    auto menu = Menu::create(closeItem, NULL);
                    menu->setPosition(Vec2::ZERO);
                    this->addChild(menu, 1);

                    queue<struct ScoreBoardAttributes> q = DBManager::getInstance()->getScoreBoard();
                    cout<<"score record count: "<<((int)q.size())<<endl;
                    
                    auto img = Sprite::create("res/backgrounds/mainbg.jpg");
                    addChild(img);
                    auto img2 = Sprite::create("res/images/honor.png");
                    
                    addChild(img2);
                    img2->setScale(visibleSize.width/img2->getContentSize().width, visibleSize.height/img2->getContentSize().height);
                    img2->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
                     auto listView = ListView::create();
                    listView->setBackGroundColor(Color3B::BLACK);
                    listView->setBackGroundColorOpacity(0.6);
                    listView->setDirection(ui::ScrollView::Direction::BOTH);
                    listView->setTouchEnabled(true);
                    listView->setBounceEnabled(true);
                    listView->setSize(Size(getContentSize().width,getContentSize().height)); //winSize.width, winSize.height/3));
                    listView->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
                    listView->setClippingEnabled(true);
                    listView->setAnchorPoint(Vec2::ZERO);
                    listView->setPosition(Vec2::ZERO);
                    listView->setSwallowTouches(false);
                    listView->pushBackCustomItem(getHeaderText());
                    /*auto wid1 = Widget::create();
                    wid1->setContentSize(spr1->getContentSize());
                    wid1->addChild(spr1);
                    */
                   
                    //listView->pushBackCustomItem(wid1);
                    unsigned int rank = 0;
                    while(q.size()){
                        struct ScoreBoardAttributes s = q.front();
                        q.pop();
                        listView->pushBackCustomItem(getContentText(&s,++rank));
                    }
                    addChild(listView);
                    auto btn = Button::create();
                    //btn->addClickEventListener(<#const ccWidgetClickCallback &callback#>)
                    return true;
                    
                }
            void menuCloseCallback(Ref* pSender)
            {
                Director::getInstance()->popScene();
                //Close the cocos2d-x game scene and quit the application
             
                /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
                
                //EventCustom customEndEvent("game_scene_close_event");
                //_eventDispatcher->dispatchEvent(&customEndEvent);
                
                
            }
                CREATE_FUNC(ScoreBoardScene);
        protected:
            /**
             @return 사용자 프로그램 설정에서 소리가 켜져있는지 유무를 리턴
             */
            bool readSoundEnable(){
                return DBManager::getInstance()->readAppSettingAsBool(DBManager::getSoundEnablerKey());
            }
            /**
             @return 내부 기록을 레이아웃 객체로 감싸 리턴
             */
            Widget* getContentText(struct ScoreBoardAttributes* attrs,int rank){
                struct ScoreBoardAttributes attr = *attrs;
                auto wid1 = Widget::create();
                stringstream ss;
                ss.fill(' ');
                ss.width(2);
                ss<<rank<<"  ";
                ss.width(10);
                
                ss<<(attr.score)<<"  ";
                ss.width(6);
                ss<<(attr.playtimesec)<<"s  ";
                ss.width(15);
                ss<<(ctime(&(attr.ts)));
                auto lbl = Label::createWithTTF((string)ss.str(), "fonts/DXSeNB-KSCpc-EUC-H.ttf", 20);
                lbl->setPosition(Vec2(lbl->getContentSize().width/2,lbl->cocos2d::Node::getContentSize().height/2));
                //lbl->setAnchorPoint(Vec2(0, 0.5f));
                wid1->addChild(lbl);
                wid1->setContentSize(Size(lbl->getContentSize().width*0.9f,lbl->getContentSize().height));
                return wid1;
            }
            /**
             @return 내부 기록의 헤더를 레이아웃 객체로 감싸 리턴
             */
            Widget* getHeaderText(){
                auto wid1 = Widget::create();
               
                auto lbl = Label::createWithTTF("순위   점수      플레이시간   기록날짜", "fonts/DXSeNB-KSCpc-EUC-H.ttf", 40);
                lbl->setPosition(Vec2(lbl->getContentSize().width/2,lbl->cocos2d::Node::getContentSize().height/2));
                //lbl->setAnchorPoint(Vec2(0, 0.5f));
                 wid1->addChild(lbl);
                wid1->setContentSize(Size(lbl->getContentSize().width*0.9f,lbl->getContentSize().height));
                return wid1;
            }
        };
}}

#endif
