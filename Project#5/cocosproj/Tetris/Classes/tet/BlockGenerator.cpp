
#include <iostream>
#include "BlockGenerator.h"
//#include "Block.h"
//class Block;
//#include "Util.h"
using namespace std;
using namespace Tetris;
namespace Tetris{
        Block* BlockGenerator::generateRandomBlock(){
            return this->generateBlock(Tetris::Util::randInt(1,7)); //randint(1,7));
            }
        Block* BlockGenerator::generateBlock(int num){
                Block* rst = NULL;
                switch(num){
                    case 1:{
                        rst = new Block(TETBLK_TYPE_I);
                        break;
                    }
                    case 2:{
                        rst = new Block(TETBLK_TYPE_L);
                        break;
                    }
                    case 3:{
                        rst = new Block(TETBLK_TYPE_J);
                        break;
                    }

                    case 4:{
                        rst = new Block(TETBLK_TYPE_M);
                        break;
                    }
                    case 5:{
                        rst = new Block(TETBLK_TYPE_Z);
                        break;
                    }
                    case 6:{
                        rst = new Block(TETBLK_TYPE_S);
                        break;
                    }
                    case 7:{
                        rst = new Block(TETBLK_TYPE_T);
                        break;
                    }

                }
                if(rst!=NULL){
                    const int rndrotcnt = Tetris::Util::randInt(0,3);
                    for(int i=0;i<rndrotcnt;i++){
                        rst->rotate();
                    }
                }
                return rst;
            }
    

}

