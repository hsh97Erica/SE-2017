
#ifndef __TETUSR_H_INC_
#define __TETUSR_H_INC_

#include <iostream>
#include "Block.hpp"
#include "BlockGenerator.hpp"
using namespace std;
using namespace Tetris;
namespace Tetris{
    namespace Users{
        class UserData{

        };
        class GameUser{
            public:
                GameUser(){
                    this->ud = NULL;
                }
                GameUser(UserData* mud){
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
                bool canSwitchBlock(){
                    return false;
                }
            private:
                unsigned short currentXpos=0;
                unsigned short currentYpos=0;
                Block* currentBlock=NULL;
                Block* nextBlock=NULL;
                Tetris::BlockGenerator bg;
                UserData* ud;
        };
    }
};
#endif
