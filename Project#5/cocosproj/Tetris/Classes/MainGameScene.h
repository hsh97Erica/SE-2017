#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__

#include "cocos2d.h"
#include "./tet/GameController.hpp"
using namespace Tetris;
class MainGame : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    GameController* gc = NULL;
    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);
    protected:GameController* gc = NULL;
};

#endif // __HELLOWORLD_SCENE_H__
