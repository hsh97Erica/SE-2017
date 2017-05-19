

#ifndef _GAMECTL_H_INC_
#define _GAMECTL_H_INC_

#include <iostream>
#include <unistd.h>
#include <vector>
#include <stack>
#include <cstring>
#include "GameUserClsDelegate.h"
#include "User.h"
#include "Block.h"
using namespace std;
using namespace Tetris;
using namespace Tetris::Delegates;
namespace Tetris{
    class InitGameInfo;

    class GameController{
        public:
            static GameController* getInstance(){
                return new GameController();
            }
            enum class GameStatus{UNKNOWN,ONGOING,END,PAUSE};
            GameController(){
                //this->init(NULL);
                //this->isEnd = false;
                this->justinit();
            }
            GameController(InitGameInfo* igi){
                //this->isEnd=false;
                this->setInitGameInfo(igi,false);
            }
       /* void setGameStatus(GameStatus mGs);
            GameStatus getGameStatus(){
                return this->gs;
            }
        unsigned char getGameHeight();
        unsigned char getGameWidth();
        */
        void setGameStatus(GameController::GameStatus mGs){
            this->gs = mGs;
        }
        GameStatus getGameStatus(){return this->gs;}
        unsigned char getGameHeight(){
            return this->gameHeight;
        }
        unsigned char getGameWidth(){
            return this->gameWidth;
        }
        void justinit();
        bool canRemoveLine(int y);
        bool isOngoing();
        void setGameStatusToOngoing();
        void setGameStatusToEnd();
        void findAndRemoveLines();
        void removeLines(stack<int> s);
        void init(InitGameInfo* igi);
        void innergameloop();
        void run();
        void setInitGameInfo(InitGameInfo* igi,bool withDelPrevMem);
        void currentblockrotate();
        void currentusermoveleft();
            bool forceend = false;
        void currentusermoveright();
        void saveBlockAndCheck(Users::GameUser* guser);
        void printposinfo();
        void play();
        void pause(){
            this->gs = GameStatus::PAUSE;
        }
        bool isPaused(){
            return this->gs==GameStatus::PAUSE;
        }
        bool isEnd(){
            return this->gs==GameStatus::END;
        }
        void resume(){
            this->gs = GameStatus::ONGOING;
        }
        bool checkEnd();
        char** getVisualizedBoard();
        virtual void printcurrentboard(){
            cout<<"[[SYSTEM]] before loading visual board..."<<endl;
            char** printboard = getVisualizedBoard();
            cout<<"[[SYSTEM]] visual board load ok"<<endl;
            if(printboard!=NULL){
                cout<<"====CURBOARD( "<<((gplaytime/60)%60)<<":"<<(gplaytime%60)<<" )===="<<endl<<endl;
                for(int i=0;i<this->gameHeight;i++){
                    for(int j=0;j<this->gameWidth;j++){
                        cout<<printboard[i][j]<<" ";
                    }
                    cout<<endl;
                    delete [] printboard[i];
                }
                cout<<endl<<endl;
            }else{
                cout<<"board is null"<<endl<<endl;
            }
        }
        bool canmoveleft();
        
        bool canmoveright();
        bool fastdropdown();

        bool candropdown();
        protected:
            unsigned char gameHeight=0;
            unsigned char gameWidth=0;
            unsigned long long gplaytime=0;
            GameStatus gs = GameController::GameStatus::UNKNOWN;
            InitGameInfo* initginfo;
            unsigned char usercount;
            vector<Users::GameUser*> gusers;
            vector<bool*> board;
        bool usercheck();
        bool saveBlockAt(Block* blk, unsigned short cury,unsigned short curx );
    };
    class InitGameInfo{
        public:

            unsigned char gameHeight;
            unsigned char gameWidth;
            short gamemode;
    };
}
#endif
