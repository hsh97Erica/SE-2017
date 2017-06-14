/**
 @file User.hpp

 */
#ifndef __TETUSR_H_INC_
#define __TETUSR_H_INC_

#include <iostream>
#include "Block.hpp"
#include "BlockGenerator.hpp"
#include "ScoreManagement.hpp"
#include <ctime>
#define _MAX_COMBO_TIME_SEC 7
using namespace std;
using namespace Tetris;
using namespace Tetris::ScoreManage;
namespace Tetris{
    namespace Users{
        class UserData{
        public:
            void saveAllData();
            void reloadAllDataFromDB();
            void saveData(long long datasecnum);
            
        };
        /**
         @class GameUser
         @brief 플레이하는 주체를 객체로서 관리
         */
        class GameUser{
            public:
                GameUser(){
                    initCls();
                    loadUserDataFromExistLocation();
                }
                GameUser(UserData* mud){
                    initCls();
                    this->ud = mud;
                }
                /**
                 @return 현재 떨어지는 블럭의 x좌표를 리턴
                 */
                unsigned short getCurrentX(){
                    return this->currentXpos;
                }
            /**
             @return 현재 떨어지는 블럭의 y좌표를 리턴
             */
                unsigned short getCurrentY(){
                    return this->currentYpos;
                }
            /**
             @return 현재 떨어지는 블럭의 x좌표를 설정
             */
                void setCurrentX(unsigned short x){
                    this->currentXpos = x;
                }
            /**
             @return 현재 떨어지는 블럭의 x좌표를 설정
             */
                void setCurrentY(unsigned short y){
                    this->currentYpos = y;
                }
            /**
             @return 다음에 떨어질 블럭 데이터 객체를 리턴
             */
                Block* getNextBlock(){
                    if(this->nextBlock==NULL){
                        
                        this->nextBlock = this->bg.generateRandomBlock();
                    }
                    return this->nextBlock;
                }
            /**
             @return 현재 떨어질 블럭 데이터 객체를 리턴
             */
                Block* getCurrentBlock(){
                     if(this->currentBlock==NULL){
                        this->currentBlock = this->bg.generateRandomBlock();
                    }
                    return this->currentBlock;
                }
            /**
             @return 다음블럭을 새로 로딩하고 현재블럭이였던 블럭을 리턴
             */
                Block* cycleBlock(){
                     Block* rst = this->getCurrentBlock();
                    this->setCurrentBlock(this->getNextBlock());
                    this->nextBlock = this->bg.generateRandomBlock();
                    return rst;
                }
                void setNextBlock(Block* blk){
                    this->nextBlock = blk;
                }
                void setCurrentBlock(Block* blk){
                    this->currentBlock=blk;
                }
            /**
             @return 현재블럭과 다음블럭을 바꿈
             */
                void switchBlock(){
                    Block* curblk=this->getCurrentBlock();
                    Block* nxtblk=this->getNextBlock();
                    this->setCurrentBlock(nxtblk);
                    this->setNextBlock(curblk);
                }
            /**
             @return 현재블럭과 다음블럭의 순서를 서로 바꿔칠수 있는지 여부
             */
                bool canSwitchBlock(vector<bool*> mapdata,const int gameHeight,const int gameWidth ,unsigned char curX,unsigned char curY){
                    Block* nxtBlk = this->getNextBlock();
                    const int blkhei = nxtBlk->getBlockSpaceHeight();
                    const int blkwid = nxtBlk->getBlockSpaceWidth();
                     bool** blkdt = nxtBlk->getBlockData();
                    for(int i=0;i<blkhei;i++){
                        for(int j=0;j<blkwid;j++){
                            if((curX+j<gameWidth&&curY+i<gameHeight&&blkdt[i][j]&&mapdata[curY+i][curX+j])||(curX+j>=gameWidth&&blkdt[i][j])){
                                return false;
                            }
                        }
                    }
                    
                    return true;
                }
            void initGameScore(){
                this->current_game_score=0;
                
            }
            /**
             @return 현재 얻은 점수를 리턴
             */
            unsigned long long getCurrentGameScore(){
                checkRequireComboReset(true);
                return this->current_game_score;
            }
            void setCurrentGameScore(const unsigned long long newscore){
                
                this->current_game_score =newscore;
            }
            /**
             @return 점수를 추가하고 추가된 총 점수를 리턴
             */
            unsigned long long accumulateCurrentGameScore(const unsigned long long addscore){
                this->current_game_score +=addscore;
                return this->current_game_score;
            }
            void resetRmCombo(){
                continous_rmlines_cnt_as_combo=0;
                resetLastestComboTime();
                cout<<endl<<"%%%%%%reset combo%%%%%%"<<endl<<endl;
            }
            /**
             @return  콤보를 쌓을수 있는 시간이 지나 콤보 숫자를 리셋이 필요한지의 유무
             */
            bool checkRequireComboReset(bool autoresetifsucceed){
                time_t curtime = time(NULL);
                if(this->getGameResumeState()){
                    const time_t timegap = curtime-this->lastest_combo_time;
                    this->lastest_combo_time = curtime-timegap;
                }
                else{
                    const bool rst =(curtime-this->lastest_combo_time)>_MAX_COMBO_TIME_SEC;
                    if(rst&&autoresetifsucceed){
                        resetRmCombo();
                    }
                    return rst;
                }
            }
            unsigned long long getRemovedLinesCount(){
                return this->rmLinesCnt;
            }
            unsigned long long accumulateRemovedLinesCount(unsigned long long addrmlines){
                this->rmLinesCnt+=addrmlines;
                checkRequireComboReset(true);
                continous_rmlines_cnt_as_combo+=addrmlines;
                resetLastestComboTime();
                return this->rmLinesCnt;
            }
            long long getCurrentComboCount(){
                checkRequireComboReset(true);
                return this->continous_rmlines_cnt_as_combo;
            }
            void setRemovedLinesCount(unsigned long long rmlines){
                const unsigned long long before = this->rmLinesCnt;
                if(rmlines<before){
                    resetRmCombo();
                }
                else{
                    checkRequireComboReset(true);
                    continous_rmlines_cnt_as_combo+=(rmlines-before);
                    resetLastestComboTime();
                }
                this->rmLinesCnt=rmlines;
            }
            void setGameResumeState(bool isResume){
                this->isGameResume = isResume;
            }
            /**
             @return GameController객체에서 가져온 게임 일시정지 상태값
             */
            bool getGameResumeState(){
                return this->isGameResume;
            }
            /**
             @return 레벨업이 가능한지 유무
             */
            bool canLevelUp(){
                unsigned long long dlv = (current_game_score/100);
                
                if(dlv>getRawLevel()){
                    cout<<"****can level up!****"<<endl;
                    return true;
                }
                return false;
            }
            /**
             @return 계산된 레벨과 현재 설정된 레벨의 차이값
             */
            unsigned long long getDeltaLv(){
                unsigned long long dlv = (current_game_score/100);
                if(dlv>getRawLevel()){
                    return dlv-getRawLevel();
                }else{
                    return 0;
                }
            }
            void setLevel(unsigned long long val){
                lv = val;
            }
            void levelupOnce(){
                lvup();
            }
            void levelup(unsigned long long uplv){
                lvup_v2(uplv);
            }
            /**
             @return gui에 보여질 레벨
             */
            unsigned long long getLevel(){
                return getRawLevel()+1;
            }
        protected:
            virtual void loadUserDataFromExistLocation(){
                
            }
            /**
             @return 내부 클래스에서 관리하고 인식할 실제 레벨값
             */
            unsigned long long getRawLevel(){
                return lv;
            }
            virtual void initCls(){
                this->initGameScore();
                this->setRemovedLinesCount(0);
                this->continous_rmlines_cnt_as_combo = 0;
                this->lv = 0;
                this->lastest_combo_time = 0;
                this->currentXpos=0;
                this->currentYpos=0;
                if(this->currentBlock!=NULL){
                    delete currentBlock;
                }
                this->currentBlock = NULL;
                
                if(this->nextBlock!=NULL){
                    delete nextBlock;
                }
                this->nextBlock = NULL;
                if(this->ud!=NULL){
                    delete ud;
                }
                this->ud = NULL;
            }
            void resetLastestComboTime(){
                this->lastest_combo_time = time(NULL);
            }
            void lvup_v2(unsigned long long uplv){
                cout<<"uplv= "<<uplv<<"  "<<"lv= "<<lv<<endl;
                lv+=uplv;
            }
            void lvup(){
                lv++;
            }
            private:
            bool isGameResume = false;
                unsigned short currentXpos=0;
                unsigned short currentYpos=0;
                Block* currentBlock=NULL;
                Block* nextBlock=NULL;
                Tetris::BlockGenerator bg;
                UserData* ud=NULL;
                unsigned long long current_game_score = 0;
                unsigned long long rmLinesCnt = 0;
                time_t lastest_combo_time = 0;
                long long continous_rmlines_cnt_as_combo = 0;
                unsigned long long lv =0;
        };
    }
};
#endif
