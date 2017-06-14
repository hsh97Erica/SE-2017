
/**
 @file GameController.hpp
 
 */

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
#include "../cocosclses/AudioSourceManager.hpp"
#include "DBManagement.hpp"
using namespace std;
using namespace Tetris;
using namespace Tetris::Delegates;
using namespace Tetris::ScoreManage;
using ScoreDBManager = Tetris::DBManagement::DBManager;
namespace Tetris{
    class InitGameInfo;
    /**
     @class GameController
     @brief 테트리스게임을 실질적으로 관리하는 클래스
     */
    class GameController{
        public:
        //static GameController* mInstance;
        /**
         @return 싱글톤형식의 GameController 객체
         */
            static GameController* getInstance(){
                static GameController ins;
                return &ins;
                //if(Tetris::GameController::mInstance==NULL){Tetris::GameController::mInstance = new GameController();}return Tetris::GameController::mInstance;
            }
            enum class GameStatus{UNKNOWN,ONGOING,END,PAUSE}; ///< 게임의 진행상태
            GameController(){
                if(!ScoreDBManager::getInstance()->isOpened()){
                    char* fileptr =(char*) (ScoreDBManager::getDBLocationForTetrisGame()) .c_str();
                    ScoreDBManager::getInstance()->open(fileptr);
                }
                //Tetris::GameController::mInstance
                this->justinit();
            }
        ~GameController(){
            ScoreDBManager::getInstance()->close();
        }
            GameController(InitGameInfo* igi){
                //this->isEnd=false;
                this->setInitGameInfo(igi,false);
            }
            void setGameStatus(GameStatus mGs){
                this->gs = mGs;
            }
        /**
         @return 현재 게임의 상태
         */
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
                    this->scorechecker =this->rmLinesCntDeltaChecker= 0;
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
                        cout<<"guser deleted"<<endl;
                        delete guser;
                    }
                    
                }
                 gusers.clear();
                    //this->gusers = new Users::GameUser*[this->usercount];
                    for(int i=0;i<this->usercount;i++){
                        this->gusers.push_back( new Users::GameUser());
                        this->gusers[i]->setCurrentY(this->gameHeight);
                        //this->gusers[i]->setCurrentY(this->gameHeight - this->gusers[i]->getCurrentBlock()->getBlockSpaceHeight());
                        this->gusers[i]->setCurrentX((this->gameWidth/2) - (this->gusers[i]->getCurrentBlock()->getBlockSpaceWidth()/2));
                    }
                getLocalUser()->setRemovedLinesCount(0);
            }
        /**
         @return y번째 라인을 지울수 있는지 여부
         */
            bool canRemoveLine(int y){
                for(int i =0;i<this->gameWidth;i++)if(!this->board[y][i])return false;
                return true;
            }
        /**
         @return 게임상태가 진행중인지 여부
         */
            bool isOngoing(){
                return this->getGameStatus()==GameStatus::ONGOING;
            }
        
            void setGameStatusToOngoing(){
                this->setGameStatus(GameStatus::ONGOING);
            }
            void setGameStatusToEnd(){
                this->setGameStatus(GameStatus::END);
                ScoreDBManager::getInstance()->saveScore(getLocalScore(),getPlayTimeAsRaw());
            }
            void findAndRemoveLines(){
                stack<int> s;
                for(int i=0;i<this->gameHeight;i++){
                    if(canRemoveLine(i)){
                        s.push(i);
                    }
                }
                if(s.size()){removeLines(s);
                    Tetris::Audio::AudioSourceManager::getInstance()->playRemovingBlocksEffect();
                   // CocosDenshion::Sim
                }
            }
        /**
         @return  게임 진행 시간이 1초이상 지났는지 여부
         */
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
        /**
         @return 현재 블럭과 다음 블럭이 바뀌였는지 성공여부
         */
        bool switchBlock(){
            GameUser* guser = getLocalUser();
            if(guser->canSwitchBlock(board,gameHeight,gameWidth, guser->getCurrentX(),guser->getCurrentY())){
                guser->switchBlock();
            }
        }
        /**
         @return  gui에 띄울 '지운 라인수'항목이 새로고침이 필요한지 여부
         */
        bool checkRmCntDisplayRefresh(){
            const bool rst = this->rmLinesCntDeltaChecker<getLocalUser()->getRemovedLinesCount();
            if(rst){
                this->rmLinesCntDeltaChecker=getLocalUser()->getRemovedLinesCount();
            }
            return rst;
        }
        /**
         @return  gui에 띄울 '점수'항목이 새로고침이 필요한지 여부
         */
        bool checkScoreDisplayRefresh(){
            const bool rst = this->scorechecker<getLocalUser()->getCurrentGameScore();
            if(rst){
                this->scorechecker=getLocalUser()->getCurrentGameScore();
            }
            return rst;
        }
        void forceSettingScore(unsigned long long sc){
            this->getLocalUser()->setCurrentGameScore(sc);
            this->scorechecker = sc;
            
        }
        /**
         @return 지운 라인수를 표현해줄 텍스트
         */
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
        /**
         @return 게임 로직을 진행시키고 최종적으로 진행1번 한뒤의 떨어지는 블럭과 저장된 블럭의 위치를 담은 메트릭스 데이터 배열
         */
        char** innergameloop(){
            char** rst = NULL;
            if(!this->usercheck())return NULL;
            for(int i=0;i<this->gusers.size();i++){
                gusers[i]->setGameResumeState(isPaused());
            }
            //bool hasTimeDelta = false;
            //cout<<"inner play time: "<<gplaytime<<endl;
            if(!this->forceend&&this->isOngoing()){
                Users::GameUser* guser = this->gusers[0];
                //unsigned char* colors = guser->getCurrentBlock()->getBlockColor()->getColorAsArray();
                rst = this->getCombinedBoard();
                //rst = this->getVisualizedBoard();
                //this->printcurrentboard();
                //delete colors;
                
                if(checkEnd()){
                    cout<<"chk end"<<endl;
                    this->setGameStatusToEnd();return rst;}
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
                getLocalUser()->checkRequireComboReset(true);
                if(this->occurtimedelta||tmp_time_delta>this->timedeltachecker){
                    //getLocalUser()->checkRequireComboReset(true);
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
            bool forceend = false; ///< 외부에서 게임을 강제로 종료시킬경우에 사용하는 변수
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
        /**
         @return 점수텍스트
         */
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
        /**
         @return 라인수 텍스트
         */
        string getRemovedLinesCountWithFormatForSomeUser(GameUser* guser,bool hasPrefixFormat){
            stringstream ss;
            if(hasPrefixFormat){
                ss<<"-지운 라인수-"<<endl;
            }
            ss<<(guser->getRemovedLinesCount());
            return ss.str();
        }
        /**
         @return  플레이한 시간을 초단위로 리턴
         */
        unsigned long long getPlayTimeAsRaw(){
            return this->gplaytime;
        }
            void play()
        { // 콘솔 플레이용 함수
                this->gs=GameStatus::ONGOING;
                while(!this->forceend&&!this->isEnd()){
                    while(!this->forceend&&this->gs==GameStatus::ONGOING){
                        cout<<"new loop"<<endl;
                        //this->printposinfo();

                        //this->printcurrentboard();
                        cout<<"printboard ok"<<endl;
                        if(checkEnd()){setGameStatusToEnd();}
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
        /**
         @return 게임 진행 상태가 일시정지인지유무
         */
         
            bool isPaused(){
                return this->gs==GameStatus::PAUSE;
            }
        /**
         @return 게임 진행 상태가 게임종료인지유무
         */
            bool isEnd(){
                return this->gs==GameStatus::END;
            }
            void resume(){
                this->gs = GameStatus::ONGOING;
            }
        /**
         @return 게임이 종료되어야하는 상황인지 아닌지
         */
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
                            cout<<"save scr in checkEnd"<<endl;
                            ScoreDBManager::getInstance()->saveScore(getLocalScore(),getPlayTimeAsRaw());
                            return true;
                        }
                    }
                }
                return false;
            }
        /**
         @return 게임 플레이 주체를 담은 배열
         */
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
        /**
         @return 저장된 블럭의 위치와 떨어지는 블럭의 위치를 하나의 메트릭스안에 포함한 2차원 배열
         */
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
        /**
         @return 콘솔용 통합 메트릭스
         */
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
        /**
         @return 블럭이 왼쪽으로 이동할 수있는지 여부
         */
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
        /**
         @return 블럭이 오른쪽으로 이동할 수있는지 여부
         */
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
        /**
          @return 블럭이 순식간에 떨어졌는지 유무
         */
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
        /**
         @return 떨어지는 블럭이 1칸 강제로 더 떨어졌는지 유무
         */
        bool forcedropdownOnce(){
            if(candropdown()){
                 Users::GameUser* guser = this->gusers[0];
                guser->setCurrentY(guser->getCurrentY()-1);
                return true;
            }
            return false;
        }
        /**
         @return 블럭이 강제로 1칸 더 떨어질 수 있는지 유뮤
         */
         
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
            /**
             @return 게임 주체가 1명 이상인지 유무
             */
            bool usercheck(){
                if(usercount==0||!this->gusers.size()||(this->gusers.size()&&this->gusers[0]==NULL)){
                    return false;
                }
                return true;
            }
        /**
         @return 떨어지는 블럭이 해당 좌표에 저장하는 명령을 수행하고 그 저장이 성공했는지유무
         */
            bool saveBlockAt(Block* blk, unsigned short cury,unsigned short curx ){
                if(!this->board.size()){
                    return false;
                }
                const int blkhg = blk->getBlockSpaceHeight();
                const int blkwd = blk->getBlockSpaceWidth();
                 bool** blddt = NULL;
                 if(cury+blkhg>this->gameHeight){
                     setGameStatusToEnd();
                     //this->gs = GameStatus::END;
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
                this->getLocalUser()->accumulateCurrentGameScore(ScoreManagement::getScoreWhenPuttingFallenDownBlock());
                return true;
            }
        
        //static GameController* mInstance;

        
    private:
        
       // static GameController* mInstance;
        
        time_t timedeltachecker=time(NULL); ///< 게임 경과시간의 차이를 계산하기위한 변수
        bool occurtimedelta = false;
        unsigned long long rmLinesCntDeltaChecker = 0; ///< 지운 줄의 수의 차이를 계산하기위한 변수
        unsigned long long scorechecker = 0; ///< 점수의 차이를 계산하기위한 변수
    };
    class InitGameInfo{
        public:

            unsigned char gameHeight;
            unsigned char gameWidth;
            short gamemode;
    };
    
    //GameController* GameController::mInstance = NULL;
}
#endif
