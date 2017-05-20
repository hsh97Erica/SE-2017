#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "tet2/GameController.hpp"
#include <iostream>
using namespace std;
using namespace Tetris;
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void startGame();
    void drawboardingui(char** board,unsigned char* clr);
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
    cocos2d::Layer* overlayblockboard=NULL;
    cocos2d::SEL_SCHEDULE mainloopfuncschedule = NULL;
};

#endif // __HELLOWORLD_SCENE_H__
