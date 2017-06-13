#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "tet2/GameController.hpp"
#include "views/NextBlockRenderView.hpp"
#include <iostream>
using namespace std;
using namespace Tetris;
//using namespace Tetris::Views;
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
        Tetris::GameController* gc = NULL;
        void play(float dt);
        void pause();
        void resume();
        void stateloop(float dt);
        void gameloop(float dt);
        void menuVisibleToggle(bool autopauseorpause);
    void freeRelativeConnectionWhenDeleting();
        virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
        virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
        cocos2d::Layer* overlayblockboard=NULL;
        cocos2d::SEL_SCHEDULE mainloopfuncschedule = NULL;
        cocos2d::Label* ptimelbl = NULL;
        cocos2d::Label* pscorelbl = NULL;
        cocos2d::Label* rmlnscntlbl = NULL;
        cocos2d::Layer* nxtblkrenderingarea = NULL;
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
        unsigned short bonus_game_lv;
        bool isInExtGame = false;
    unsigned int effect_ext_2048gm;
    bool readSoundEnable();
    
    string getMenuTextSoundEnabler(bool enable);
};

#endif // __HELLOWORLD_SCENE_H__
