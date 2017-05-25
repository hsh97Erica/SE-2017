

#ifndef _GAMECTL_H_INC_
#define _GAMECTL_H_INC_

#include <iostream>
#include <unistd.h>
#include <vector>
#include <stack>
#include <cstring>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include "GameUserClsDelegate.hpp"
#include "ScoreManagement.hpp"
#include "User.hpp"
#include "Block.hpp"
#include "../HelloWorldScene.h"

using namespace std;
using namespace Tetris;
using namespace Tetris::Delegates;
using namespace Tetris::ScoreManage;
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
            void setGameStatus(GameStatus mGs){
                this->gs = mGs;
            }
            GameStatus getGameStatus(){
                return this->gs;
            }
            unsigned char getGameHeight(){
                return gameHeight;
            }
            unsigned char getGameWidth(){
                return gameWidth;
            }
            void justinit(){
                    while(this->board.size()){
                        this->board.pop_back();
                    }
                    timedeltachecker = time(NULL);
                occurtimedelta=false;
                    this->gameHeight = 20;
                    this->gameWidth = 8;
                    this->gs = GameStatus::UNKNOWN;
                
                    //rmLinesCnt=0;
                    //this->board = new bool*[this->gameHeight];
                    for(int i=0;i<this->gameHeight;i++){
                        bool* newrow = new bool[this->gameWidth];
                        memset(newrow,false,sizeof(bool)*this->gameWidth);
                        this->board.push_back( newrow);
                    }
                    this->gplaytime = 0;
                    this->usercount=1;
                if(gusers.size()){
                    while(!gusers.empty()){
                        GameUser* guser = gusers[0];
                        gusers.erase(gusers.begin());
                        delete guser;
                    }
                    gusers.clear();
                }
                    //this->gusers = new Users::GameUser*[this->usercount];
                    for(int i=0;i<this->usercount;i++){
                        this->gusers.push_back( new Users::GameUser());
                        this->gusers[i]->setCurrentY(this->gameHeight);
                        //this->gusers[i]->setCurrentY(this->gameHeight - this->gusers[i]->getCurrentBlock()->getBlockSpaceHeight());
                        this->gusers[i]->setCurrentX((this->gameWidth/2) - (this->gusers[i]->getCurrentBlock()->getBlockSpaceWidth()/2));
                    }
                getLocalUser()->setRemovedLinesCount(0);
            }
            bool canRemoveLine(int y){
                for(int i =0;i<this->gameWidth;i++)if(!this->board[y][i])return false;
                return true;
            }
            bool isOngoing(){
                return this->getGameStatus()==GameStatus::ONGOING;
            }
            void setGameStatusToOngoing(){
                this->setGameStatus(GameStatus::ONGOING);
            }
            void setGameStatusToEnd(){
                this->setGameStatus(GameStatus::END);
            }
            void findAndRemoveLines(){
                stack<int> s;
                for(int i=0;i<this->gameHeight;i++){
                    if(canRemoveLine(i)){
                        s.push(i);
                    }
                }
                if(s.size())removeLines(s); 
            }
            bool haveTimeDelta(){
                bool rst = this->occurtimedelta;
                if(rst){
                this->occurtimedelta = false;
                }
                return rst;
            }
        bool requireRemovedLinesCountRefresh(){
            return false;
        }
            void removeLines(stack<int> s){
                const int restorecount = (int)s.size();
                int scre = 0;
                while(s.size()){
                    const int pos = s.top();
                    s.pop();
                    bool* tmp = this->board[pos];
                    this->board.erase(this->board.begin()+pos);
                    delete tmp;
                    bool* newrow = new bool[this->gameWidth];
                    memset(newrow,false,this->gameWidth*sizeof(bool));
                    this->board.push_back(newrow);
                    scre+= (ScoreManagement::getDefaultScoreWhenRemovingOneLine()+ ScoreManagement::getScoreGapWhenRemovingOneLine());
                }
                this->getLocalUser()->accumulateRemovedLinesCount(restorecount);
                const float comboscore =ScoreManagement::getComboScore(getLocalUser()->getCurrentComboCount());
                const int accscr = scre+(int)comboscore;
                cout<<"total accumu score: "<<(accscr)<<endl<<"scre: "<<scre<<endl<<"combo score: "<<comboscore<<endl<<"cur combo cnt: "<<(getLocalUser()->getCurrentComboCount())<<endl;
                  getLocalUser()->accumulateCurrentGameScore(accscr );
            }
            void init(InitGameInfo* igi){
                if(igi==NULL){
                    this->justinit();
                }
            }
        bool switchBlock(){
            GameUser* guser = getLocalUser();
            if(guser->canSwitchBlock(board,gameHeight,gameWidth, guser->getCurrentX(),guser->getCurrentY())){
                guser->switchBlock();
            }
        }
        bool checkRmCntDisplayRefresh(){
            const bool rst = this->rmLinesCntDeltaChecker<getLocalUser()->getRemovedLinesCount();
            if(rst){
                this->rmLinesCntDeltaChecker=getLocalUser()->getRemovedLinesCount();
            }
            return rst;
        }
        bool checkScoreDisplayRefresh(){
            const bool rst = this->scorechecker<getLocalUser()->getCurrentGameScore();
            if(rst){
                this->scorechecker=getLocalUser()->getCurrentGameScore();
            }
            return rst;
        }
        string getRemovedLinesCountWithFormat(bool withCategoryFormat){
            return getRemovedLinesCountWithFormatForSomeone(getLocalUser(),withCategoryFormat);
        }
        string getRemovedLinesCountWithFormatForSomeone(GameUser* guser,bool withCategoryFormat){
            stringstream ss;
            if(withCategoryFormat){
                ss<<"-지운 줄수-"<<endl;
                
            }
            ss<<(guser->getRemovedLinesCount());
            return ss.str();
        }
        char** innergameloop(){
            char** rst = NULL;
            //bool hasTimeDelta = false;
            //cout<<"inner play time: "<<gplaytime<<endl;
            if(!this->forceend&&this->isOngoing()){
                if(!this->usercheck())return NULL;
                Users::GameUser* guser = this->gusers[0];
                //unsigned char* colors = guser->getCurrentBlock()->getBlockColor()->getColorAsArray();
                rst = this->getCombinedBoard();
                //rst = this->getVisualizedBoard();
                //this->printcurrentboard();
                //delete colors;
                
                if(checkEnd()){this->setGameStatusToEnd();return rst;}
                this->findAndRemoveLines();
                time_t tmp_time_delta = time(NULL);
                //cout<<"==tm info=="<<endl<<tmp_time_delta<<endl<<(this->timedeltachecker)<<endl<<endl;
                if(this->candropdown()){
                    if(tmp_time_delta>this->timedeltachecker){
                        guser->setCurrentY(guser->getCurrentY()-1);
                        this->timedeltachecker = tmp_time_delta;
                        this->occurtimedelta=true;
                        
                    }
                }else{
                    this->saveBlockAndCheck(guser);
                    cout<<"save block ok"<<endl;
                }
                if(this->occurtimedelta||tmp_time_delta>this->timedeltachecker){
                    getLocalUser()->checkRequireComboReset(true);
                    srand(time(NULL));
                    gplaytime++;
                     this->timedeltachecker = tmp_time_delta;
                }
                //cout<<"1 loop end"<<endl;
            }
            
            return rst;
        }
            void run(){
                this->justinit();
                //this->init(NULL);
                this->play();
            }
            void setInitGameInfo(InitGameInfo* igi,bool withDelPrevMem){
                if(igi!=NULL){
                    if(withDelPrevMem&&this->initginfo!=NULL){
                        delete this->initginfo;
                    }
                    this->initginfo = igi;
                }
            }
            void currentblockrotate(){
                if(!this->usercheck())return;
                Users::GameUser* guser = this->gusers[0];
                //cout<<"chk"<<endl;
                //cout<<"call currentblockrotate()"<<endl;
                
                if(guser->getCurrentBlock()->canRotate(this->board,this->gameWidth,guser->getCurrentY(),guser->getCurrentX())){
                    cout<<"aa rotate"<<endl;
                guser->getCurrentBlock()->rotate();
                }
            }
            void currentusermoveleft(){
                if(!this->usercheck())return;
                Users::GameUser* guser = this->gusers[0];
                if(this->canmoveleft()){
                    guser->setCurrentX(guser->getCurrentX()-1);
                }
            }
            bool forceend = false;
            void currentusermoveright(){
                if(!this->usercheck())return;
                Users::GameUser* guser = this->gusers[0];
               // cout<<"before check canmoveright in currentusermoveright"<<endl;
                if(this->canmoveright()){
                    guser->setCurrentX(guser->getCurrentX()+1);
                }
            }
            void saveBlockAndCheck(Users::GameUser* guser){
                Block* savableblock = guser->cycleBlock();
                if(saveBlockAt(savableblock,guser->getCurrentY(),guser->getCurrentX())){
                       // guser->setCurrentY(this->gameHeight - guser->getCurrentBlock()->getBlockSpaceHeight());
                        guser->setCurrentX((this->gameWidth/2) - (guser->getCurrentBlock()->getBlockSpaceWidth()/2));
                        delete savableblock;
                        guser->setCurrentY(this->gameHeight);
                        //if(checkEnd())this->gs = GameStatus::END;
                }
                
            }
            void printposinfo(){
                cout<<"y: "<<this->gusers[0]->getCurrentY()<<"  x: "<<this->gusers[0]->getCurrentX()<<endl;
            }
        string getPlayTimeWithFormat(bool hasPrefixFormat){
            stringstream ss;
            if(hasPrefixFormat){
                ss<<"-경과시간-"<<endl;
            }
            unsigned long long hour,minute,second;
            second = this->gplaytime%60;
            minute = ((this->gplaytime)/60)%60;
            hour = (this->gplaytime/3600)%60;
            ss.fill('0');
            ss.width(2);
            ss<<hour;
            ss<<":";
            ss.width(2);
            ss<<minute;
            ss<<":";
            ss.width(2);
            ss<<second;
            return ss.str();
        }
        string getScoreWithFormatForLocalUser(bool hasPrefixFormat){
            return getScoreWithFormatForSomeUser(this->getLocalUser(),hasPrefixFormat);
        }
        string getScoreWithFormatForSomeUser(GameUser* guser,bool hasPrefixFormat){
            stringstream ss;
            if(hasPrefixFormat){
                ss<<"-점수-"<<endl;
            }
            ss<<(guser->getCurrentGameScore());
            return ss.str();
        }
        string getRemovedLinesCountWithFormatForLocalUser(bool hasPrefixFormat){
            return getRemovedLinesCountWithFormatForSomeUser(this->getLocalUser(),hasPrefixFormat);
        }
        string getRemovedLinesCountWithFormatForSomeUser(GameUser* guser,bool hasPrefixFormat){
            stringstream ss;
            if(hasPrefixFormat){
                ss<<"-지운 라인수-"<<endl;
            }
            ss<<(guser->getRemovedLinesCount());
            return ss.str();
        }
        unsigned long long getPlayTimeAsRaw(){
            return this->gplaytime;
        }
            void play(){
                this->gs=GameStatus::ONGOING;
                while(!this->forceend&&!this->isEnd()){
                    while(!this->forceend&&this->gs==GameStatus::ONGOING){
                        cout<<"new loop"<<endl;
                        //this->printposinfo();

                        //this->printcurrentboard();
                        cout<<"printboard ok"<<endl;
                        if(checkEnd()){this->gs = GameStatus::END;}
                        cout<<"check end ok"<<endl;
                        if(!this->usercheck())break;
                        Users::GameUser* guser = this->gusers[0];
                        this->findAndRemoveLines();
                        cout<<"rm ln ok"<<endl;
                        if(this->candropdown()){
                            guser->setCurrentY(guser->getCurrentY()-1);
                            cout<<"drop if inner ok"<<endl;
                        }else{
                            this->saveBlockAndCheck(guser);
                            cout<<"save block ok"<<endl;
                        }
                        cout<<"drop down ok"<<endl;
                        gplaytime++;
                        usleep(1000000);
                        cout<<"1 loop end"<<endl;
                    }
                    if(this->gs!=GameStatus::END){
                        usleep(1000000/2);
                    }
                }
                cout<<endl<<endl<<"program finish"<<endl;
            }
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
            bool checkEnd(){
                if(!this->usercheck())return true;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                Block* blk = guser->getCurrentBlock();
                const int blkhg = blk->getBlockSpaceHeight();
                const int blkwd = blk->getBlockSpaceWidth();
                bool** blddt = NULL;
                blddt= blk->getBlockData();
                for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(cury+i<this->gameHeight&&blddt[i][j]&&board[cury+i][curx+j]){
                            return true;
                        }
                    }
                }
                return false;
            }
        vector<Users::GameUser*> getUsers(){
            return this->gusers;
        }
        Users::GameUser* getLocalUser(){
            if(getUsers().size()&&getUsers()[0]!=NULL){
                return getUsers()[0];
            }
            else{
                return NULL;
            }
        }
        unsigned long long getLocalScore(){
            return getLocalUser()->getCurrentGameScore();
        }
        
        char** getCombinedBoard(){
            if(this->board.size()){
                char** printboard = new char*[this->gameHeight];
                for(int i=0;i<this->gameHeight;i++){
                    printboard[i] = new char[this->gameWidth];
                    for(int j=0;j<this->gameWidth;j++){
                        printboard[i][j] = this->board[i][j]?'1':'0';
                    }
                }
                if(!this->usercheck())return NULL;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                Block* curblk = guser->getCurrentBlock();
                const int blkhg = curblk->getBlockSpaceHeight();
                const int blkwd = curblk->getBlockSpaceWidth();
                bool** blddt = NULL;
                blddt= curblk->getBlockData();
                for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(i+cury<this->gameHeight&&blddt[i][j]){
                            printboard[(i+cury)][j+curx] = '2';
                        }
                    }
                }
                return printboard;
            }
            else{
                return NULL;
            }
        }
            char** getVisualizedBoard(){
                if(this->board.size()){
                    char** printboard = new char*[this->gameHeight];
                    for(int i=0;i<this->gameHeight;i++){
                        printboard[i] = new char[this->gameWidth];
                        for(int j=0;j<this->gameWidth;j++){
                            printboard[i][j] = this->board[this->gameHeight-1-i][j]?'1':'0';
                        }
                    }
                    if(!this->usercheck())return NULL;
                    Users::GameUser* guser = this->gusers[0];
                    unsigned short cury = guser->getCurrentY();
                    unsigned short curx = guser->getCurrentX();
                    Block* curblk = guser->getCurrentBlock();
                    const int blkhg = curblk->getBlockSpaceHeight();
                    const int blkwd = curblk->getBlockSpaceWidth();
                     bool** blddt = NULL;
                    blddt= curblk->getBlockData();
                    for(int i=0;i<blkhg;i++){
                        for(int j=0;j<blkwd;j++){
                            if(i+cury<this->gameHeight&&blddt[i][j]){
                                printboard[this->gameHeight-1-(i+cury)][j+curx] = '2';
                            }
                        }
                    }
                    return printboard;
                }
                else{
                    return NULL;
                }
            }
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
            bool canmoveleft(){
                 if(!this->usercheck())return false;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                if(curx==0){
                    return false;
                }
                curx--;
                Block* curblk = guser->getCurrentBlock();
                const int blkhg = curblk->getBlockSpaceHeight();
                const int blkwd = curblk->getBlockSpaceWidth();
                 bool** blddt = NULL;
             blddt= curblk->getBlockData();
             for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(i+cury<this->gameHeight&&blddt[i][j]&&board[i+cury][curx+j]){
                            return false;
                        }
                    }
                }
                return true;
            }
            
            bool canmoveright(){
                if(!this->usercheck())return false;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                curx++;
                Block* curblk = guser->getCurrentBlock();
                const int blkhg = curblk->getBlockSpaceHeight();
                const int blkwd = curblk->getBlockSpaceWidth();
                if(curx>getGameWidth()-blkwd){
                    return false;
                }
                bool** blddt = NULL;
             blddt= curblk->getBlockData();
             for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(i+cury<this->gameHeight && curx+j<this->gameWidth && blddt[i][j]&&board[i+cury][curx+j]){
                            return false;
                        }
                    }
                }
                return true;
            }
            bool fastdropdown(){
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                bool opt = false;
                while(candropdown()){
                    if(!opt){
                        opt = true;
                    }
                    guser->setCurrentY(guser->getCurrentY()-1);
                }
                if(opt){
                    this->saveBlockAndCheck(guser);
                }
                return opt;
            }
        bool forcedropdownOnce(){
            if(candropdown()){
                 Users::GameUser* guser = this->gusers[0];
                guser->setCurrentY(guser->getCurrentY()-1);
                return true;
            }
            return false;
        }
            bool candropdown(){
                if(!this->usercheck())return false;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                if(cury==0){
                    return false;
                }
                cury--;
                Block* curblk = guser->getCurrentBlock();
                const int blkhg = curblk->getBlockSpaceHeight();
                const int blkwd = curblk->getBlockSpaceWidth();
                bool** blddt = NULL;
                blddt= curblk->getBlockData();
                for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(i+cury<this->gameHeight&&blddt[i][j]&&board[i+cury][curx+j]){
                            return false;
                        }
                    }
                }
                return true;
            }
        protected:
            unsigned char gameHeight=0;
            unsigned char gameWidth=0;
            unsigned long long gplaytime=0;
            GameStatus gs = GameController::GameStatus::UNKNOWN;
            InitGameInfo* initginfo;
            unsigned char usercount;
            vector<Users::GameUser*> gusers;
            vector<bool*> board;
        
            bool usercheck(){
                if(usercount==0||!this->gusers.size()||(this->gusers.size()&&this->gusers[0]==NULL)){
                    return false;
                }
                return true;
            }
            bool saveBlockAt(Block* blk, unsigned short cury,unsigned short curx ){
                if(!this->board.size()){
                    return false;
                }
                const int blkhg = blk->getBlockSpaceHeight();
                const int blkwd = blk->getBlockSpaceWidth();
                 bool** blddt = NULL;
                 if(cury+blkhg>this->gameHeight){
                     this->gs = GameStatus::END;
                     //this->isEnd =true;
                    return false;
                 }
                blddt= blk->getBlockData();
                for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(blddt[i][j]&&i+cury<this->gameHeight){
                            board[i+cury][j+curx] = true;
                        }
                    }
                }
                return true;
            }
    private:
        time_t timedeltachecker=time(NULL);
        bool occurtimedelta = false;
        unsigned long long rmLinesCntDeltaChecker = 0;
        unsigned long long scorechecker = 0;
    };
    class InitGameInfo{
        public:

            unsigned char gameHeight;
            unsigned char gameWidth;
            short gamemode;
    };
}
#endif
