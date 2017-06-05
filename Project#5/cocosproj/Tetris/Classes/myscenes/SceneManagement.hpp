#ifndef _TET_SCENEMGRMNT_H_INC_
#define _TET_SCENEMGRMNT_H_INC_
#include "cocos2d.h"
#include "MainMenuScene.hpp"
#include "../HelloWorldScene.h"
#include "../externalgames/ CodeLadyJJY/game2048/GameScene.hpp"
using Tetris::Cocos2dScenes::MainMenuScene;
//using CodeLadyJJY::game2048::GameScene;
using cocos2d::Scene;
namespace Tetris{
 namespace Cocos2dScenes{
    class SceneInstanceManager{
        public:
            static SceneInstanceManager* getInstance(){
                SceneInstanceManager ins;
                return &ins;
            }
        static Scene* createMainMenuScene(){
            return MainMenuScene::createScene();
        }
        static Scene* createMainTetrisScene(){
            return HelloWorld::createScene();
        }
        static Scene* createGame2048Scene(){
            return CodeLadyJJY::game2048::GameScene::createScene();
        }
        static CodeLadyJJY::game2048::GameScene* createGame2048Layer(){
            return CodeLadyJJY::game2048::GameScene::create();
        }
        Scene* getStoredMainMenuScene(){
            if(mmsc==NULL){
                mmsc =SceneInstanceManager::createMainMenuScene();
            }
            return mmsc;
        }
        Scene* getStoredMainTetrisScene(){
            if(tetrisScene==NULL){
                tetrisScene =SceneInstanceManager::createMainTetrisScene();
            }
            return tetrisScene;
        }
        
        protected:
        Scene* mmsc = NULL;
        Scene* tetrisScene = NULL;
    };
    class SceneManagement{
    public:
        
        SceneInstanceManager* getSceneInstanceManager(){
            if(sceneInsMgr==NULL){
                sceneInsMgr = SceneInstanceManager::getInstance();
            }
            return sceneInsMgr;
        }
        static SceneManagement* getInstance(){
            SceneManagement ins;
            return &ins;
        }
    protected:
        
        SceneInstanceManager* sceneInsMgr = NULL;
    };

 }
}

#endif
