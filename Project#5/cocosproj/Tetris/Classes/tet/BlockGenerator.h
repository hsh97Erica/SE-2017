#ifndef _BLKGEN_H_INC_
#define _BLKGEN_H_INC_

#include <iostream>
#include "Block.h"
//class Block;
#include "Util.h"
using namespace std;
using namespace Tetris;
namespace Tetris{
    class BlockGenerator{
        public:
        Block* generateRandomBlock();
        Block* generateBlock(int num);
    };

}
#endif
