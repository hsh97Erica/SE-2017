#ifndef _SCR_BRD_SCENE_H_INC_
#define _SCR_BRD_SCENE_H_INC_

#include "cocos2d.h"
#include <iostream>
#include <queue>
#include <string>
#include "../tet2/BaseDBManagement.hpp"
#include "../tet2/DBManagement.hpp"
using namespace Tetris::DBManagement;
namespace Tetris{
    namespace Cocos2dScenes{
        class ScoreBoardScene : public cocos2d::Scene{
            public:
                static cocos2d::Scene* createScene(){
                    ScoreBoardScene* scene = ScoreBoardScene::create();
                    return scene;
                }
                virtual bool init(){
                    if ( !Scene::init() ){
                        return false;
                    }
                    if(!ScoreDBManager::getInstance()->isOpened()){
                        char* fileptr =(char*) (ScoreDBManager::getDBLocationForTetrisGame()) .c_str();
                        ScoreDBManager::getInstance()->open(fileptr);
                    }
                    queue<struct ScoreBoardAttributes> q = DBManager::getInstance()->getScoreBoard();
                    while(q.size()){
                        struct ScoreBoardAttributes s = q.front();
                        q.pop();
                        cout<<"read score: "<<(s.score)<<endl;
                    }
                    auto img = Sprite::create("res/backgrounds/mainbg.jpg");
                    addChild(img);
                    return true;
                }
                CREATE_FUNC(ScoreBoardScene);
        };
}}

#endif
