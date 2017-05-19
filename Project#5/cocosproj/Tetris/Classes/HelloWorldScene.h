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

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    protected:
        void makeField();
        Tetris::GameController* gc = NULL;
        void play();
        void pause();
        void resume();
        void stateloop();
        void gameloop();
};

#endif // __HELLOWORLD_SCENE_H__
