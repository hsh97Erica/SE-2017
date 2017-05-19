
#include <iostream>
#include "User.h"
//#include "Block.h"
//#include "BlockGenerator.h"
using namespace std;
using namespace Tetris;
namespace Tetris{
    namespace Users{
        unsigned short GameUser::getCurrentX(){
                    return this->currentXpos;
                }
        unsigned short GameUser::getCurrentY(){
                    return this->currentYpos;
                }
                void GameUser::setCurrentX(unsigned short x){
                    this->currentXpos = x;
                }
                void GameUser::setCurrentY(unsigned short y){
                    this->currentYpos = y;
                }
        Block* GameUser::getNextBlock(){
                    if(this->nextBlock==NULL){
                        
                        this->nextBlock = this->bg.generateRandomBlock();
                    }
                    return this->nextBlock;
                }
        Block* GameUser::getCurrentBlock(){
                     if(this->currentBlock==NULL){
                        this->currentBlock = this->bg.generateRandomBlock();
                    }
                    return this->currentBlock;
                }

        Block* GameUser::cycleBlock(){
                     Block* rst = this->getCurrentBlock();
                    this->setCurrentBlock(this->getNextBlock());
                    this->nextBlock = this->bg.generateRandomBlock();
                    return rst;
                }
                void GameUser::setNextBlock(Block* blk){
                    this->nextBlock = blk;
                }
                void GameUser::setCurrentBlock(Block* blk){
                    this->currentBlock=blk;
                }
                void GameUser::switchBlock(){
                    Block* curblk=this->getCurrentBlock();
                    Block* nxtblk=this->getNextBlock();
                    this->setCurrentBlock(nxtblk);
                    this->setNextBlock(curblk);
                }
        bool GameUser::canSwitchBlock(){
                    return false;
                }
           
    }
};

