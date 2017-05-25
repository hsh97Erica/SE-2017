
#ifndef __TETUSR_H_INC_
#define __TETUSR_H_INC_

#include <iostream>
#include "Block.hpp"
#include "BlockGenerator.hpp"
#include <ctime>
#define _MAX_COMBO_TIME_SEC 4
using namespace std;
using namespace Tetris;
namespace Tetris{
    namespace Users{
        class UserData{
        public:
            void saveAllData();
            void reloadAllDataFromDB();
            void saveData(long long datasecnum);
            
        };
        class GameUser{
            public:
                GameUser(){
                    initCls();
                }
                GameUser(UserData* mud){
                    initCls();
                    this->ud = mud;
                }
            
                unsigned short getCurrentX(){
                    return this->currentXpos;
                }
                unsigned short getCurrentY(){
                    return this->currentYpos;
                }
                void setCurrentX(unsigned short x){
                    this->currentXpos = x;
                }
                void setCurrentY(unsigned short y){
                    this->currentYpos = y;
                }
                Block* getNextBlock(){
                    if(this->nextBlock==NULL){
                        
                        this->nextBlock = this->bg.generateRandomBlock();
                    }
                    return this->nextBlock;
                }
                Block* getCurrentBlock(){
                     if(this->currentBlock==NULL){
                        this->currentBlock = this->bg.generateRandomBlock();
                    }
                    return this->currentBlock;
                }

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
                void switchBlock(){
                    Block* curblk=this->getCurrentBlock();
                    Block* nxtblk=this->getNextBlock();
                    this->setCurrentBlock(nxtblk);
                    this->setNextBlock(curblk);
                }
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
            unsigned long long getCurrentGameScore(){
                return this->current_game_score;
            }
            void setCurrentGameScore(const unsigned long long newscore){
                this->current_game_score =newscore;
            }
            unsigned long long accumulateCurrentGameScore(const unsigned long long addscore){
                this->current_game_score +=addscore;
                return this->current_game_score;
            }
            void resetRmCombo(){
                continous_rmlines_cnt_as_combo=0;
            }
            bool checkRequireComboReset(){
                time_t curtime = time(NULL);
                return (curtime-this->lastest_combo_time)<=_MAX_COMBO_TIME_SEC;
            }
            unsigned long long getRemovedLinesCount(){
                return this->rmLinesCnt;
            }
            unsigned long long accumulateRemovedLinesCount(unsigned long long addrmlines){
                this->rmLinesCnt+=addrmlines;
            }
            void setRemovedLinesCount(unsigned long long rmlines){
                this->rmLinesCnt=rmlines;
            }
        protected:
            virtual void initCls(){
                this->initGameScore();
                this->setRemovedLinesCount(0);
                this->continous_rmlines_cnt_as_combo = 0;
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
            private:
            
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
        };
    }
};
#endif
