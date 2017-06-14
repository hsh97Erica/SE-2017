/**
 @file HelloWorldScene.h
 @brief 메인 게임 창 헤더
 */
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "tet2/GameController.hpp"
#include "views/NextBlockRenderView.hpp"
#include <iostream>
using namespace std;
using namespace Tetris;
//using namespace Tetris::Views;
/**
 @class HelloWorld
 @brief 메인 게임창 클래스
 */
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuDrawerClickCallback(cocos2d::Ref* pSender);
    void gameforceResumeMenuCallback(Ref* pSender);
    void gameforcePauseMenuCallback(Ref* pSender);
    void gameRestartGameCallback(Ref* pSender);
    void menuSoundEnableCallback(cocos2d::Ref* pSender);
    void startGame();
    void drawboardingui(char** board,unsigned char* clr);
    MenuItemLabel* sounditem = NULL;
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    protected:
        void makeField();
        Tetris::GameController* gc = NULL; ///< 게임 관리 객체 변수
        void play(float dt);
        void pause();
        void resume();
        void stateloop(float dt);
        void gameloop(float dt);
        void levelup(float dt);
        void menuVisibleToggle(bool autopauseorpause);
    void freeRelativeConnectionWhenDeleting();
        virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
        virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
        cocos2d::Layer* overlayblockboard=NULL;
        cocos2d::SEL_SCHEDULE mainloopfuncschedule = NULL;
        cocos2d::Label* ptimelbl = NULL; ///< 플레이시간을 보여주는 뷰의 객체 변수
        cocos2d::Label* pscorelbl = NULL; ///< 점수를 보여주는 뷰의 객체 변수
        cocos2d::Label* rmlnscntlbl = NULL; ///< 지운 줄 수를 보여주는 뷰의 객체 변수
        cocos2d::Layer* nxtblkrenderingarea = NULL; ///< 플레이시간을 보여주는 뷰의 객체 변수
        cocos2d::Menu* gameoptionmenu = NULL;
        cocos2d::Menu* generateOptionMenu();
        cocos2d::Menu* generateGameOverMenu();
       // cosos2d::
        Views::NextBlockRenderBehavior* nxtblkrv = NULL;
        bool menuviewadded = false;
        void addOrRemove2048GameView(bool remove);
        cocos2d::EventListenerKeyboard* k_listener = NULL;
        cocos2d::EventListenerKeyboard* createNewKListener();
        void removeKListenerForMainGame();
    void registerKListenerForMainGame();
        unsigned short bonus_game_lv; ///< 보너스 게임이 나타날 최소조건의 점수가 2의 승수 이상임을 체크하는 변수
        bool isInExtGame = false; ///< 보너스게임의 실행 여부
    unsigned int effect_ext_2048gm; ///< 보너스게임의 bgm을 관리하는 id
    bool readSoundEnable();
    
    string getMenuTextSoundEnabler(bool enable);
};

#endif // __HELLOWORLD_SCENE_H__
