#ifndef _BLKGEN_H_INC_
#define _BLKGEN_H_INC_

#include <iostream>
#include "Block.hpp"
//class Block;
#include "Util.hpp"

using namespace std;
using namespace Tetris;
namespace Tetris{
    class BlockGenerator{
        public:
            Block* generateRandomBlock(){
                cout<<"genrandblk"<<endl;
                return this->generateBlock(TetrisUtil::randInt(1,7)); //randint(1,7));
            }
            Block* generateBlock(const int num){
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
                    const int rndrotcnt = TetrisUtil::randInt(0,3);
                    for(int i=0;i<rndrotcnt;i++){
                        rst->rotate();
                    }
                }
                return rst;
            }
    };

}
#endif
