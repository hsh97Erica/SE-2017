/**
 @file SceneManagement.hpp
 @breif
 */
#ifndef _TET_SCENEMGRMNT_H_INC_
#define _TET_SCENEMGRMNT_H_INC_
#include "cocos2d.h"
#include "MainMenuScene.hpp"
#include "../HelloWorldScene.h"
#include "ScoreBoardScene.hpp"
#include "../externalgames/ CodeLadyJJY/game2048/GameScene.hpp"
using Tetris::Cocos2dScenes::MainMenuScene;
using Tetris::Cocos2dScenes::ScoreBoardScene;
//using CodeLadyJJY::game2048::GameScene;
using cocos2d::Scene;
namespace Tetris{
 namespace Cocos2dScenes{
     /**
      @class SceneInstanceManager
      @brief 창 클래스를 관리하는 클래스
      */
    class SceneInstanceManager{
        public:
            static SceneInstanceManager* getInstance()
        { // 싱글톤 방식
                SceneInstanceManager ins;
                return &ins;
            }
        /**
         @return 메인 메뉴 창객체를 리턴
         */
        static Scene* createMainMenuScene(){
            return MainMenuScene::createScene();
        }
        /**
         @return 메인 게임 창객체를 리턴
         */
        static Scene* createMainTetrisScene(){
            return HelloWorld::createScene();
        }
        /**
         @return 보너스 게임 창객체를 리턴
         */
        static Scene* createGame2048Scene(){
            return CodeLadyJJY::game2048::GameScene::createScene();
        }
        /**
         @return 보너스 게임 레이아웃객체를 리턴
         */
        static CodeLadyJJY::game2048::GameScene* createGame2048Layer(){
            return CodeLadyJJY::game2048::GameScene::create();
        }
        /**
         @return 메인 메뉴 창객체를 리턴
         */
        static Scene* createScoreBoardScene(){
            return ScoreBoardScene::createScene();
        }
        /**
         @return 메인 메뉴 창객체를 리턴(창 전환시 에러떠서 안씀)
         */
        Scene* getStoredMainMenuScene(){
            if(mmsc==NULL){
                mmsc =SceneInstanceManager::createMainMenuScene();
            }
            return mmsc;
        }
        /**
         @return 메인 게임 창객체를 리턴(창 전환시 에러떠서 안씀)
         */
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
