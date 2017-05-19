
#ifndef __TETUSR_H_INC_
#define __TETUSR_H_INC_

#include <iostream>
#include "Block.h"
#include "BlockGenerator.h"
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
            unsigned short getCurrentX();
            unsigned short getCurrentY();
            void setCurrentX(unsigned short x);
            void setCurrentY(unsigned short y);
            Block* getNextBlock();
            Block* getCurrentBlock();
            Block* cycleBlock();
            void setNextBlock(Block* blk);
            void setCurrentBlock(Block* blk);
            void switchBlock();
            bool canSwitchBlock();
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
