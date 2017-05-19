
#ifndef _BLK_H_INC_
#define _BLK_H_INC_

//class GameUser;
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include "Util.h"
//#include "GameUserClsDelegate.h"
//#include "User.h"
using namespace std;
using namespace Tetris;
//using namespace Tetris::Delegates;
//using namespace Tetris::Users;
#define TETBLK_TYPE_I 10
#define TETBLK_TYPE_L 20
#define TETBLK_TYPE_J 1+TETBLK_TYPE_L
#define TETBLK_TYPE_M 30
#define TETBLK_TYPE_Z 40
#define TETBLK_TYPE_S 1+TETBLK_TYPE_Z
#define TETBLK_TYPE_T 50

namespace Tetris{
    //namespace Delegates{class GameUserClsDelegate;};
     class Block{
        public:
        Block(int type){
            this->spanshape(type);
            this->setBlockColor(this->genRandomColor());
        }
        ~Block(){
            if(this->blkspc!=NULL){
                for(int i=0;i<this->blkary_height;i++){
                    delete [] this->blkspc[i];
                }
                delete [] this->blkspc;
            }
        }
        Block(int type,unsigned int color){
            this->spanshape(type);
            this->setBlockColor(color);
        }
        void setBlockColor(unsigned int clr);
        unsigned int getBlockColorAsRaw();
        unsigned int genRandomColor();
        unsigned int genSampleColor(const unsigned char samplenum);

        string getBlockColorAsHexString();
        int getBlockSpaceSize();
        int getBlockSpaceHeight();
        int getBlockSpaceWidth();
        bool** getBlockData();
        int getBlockType();
        void freeblockspace(bool*** spc);
        bool** getRotatedSpaceData();
        bool canRotate(const vector<bool*> mapdata,const unsigned char boardwidth, unsigned char cury,unsigned char curx );
        bool rotate();
        bool roatespec();
        
        void printspace();
        private:
            int blktype;
            int blkary_height;
            int blkary_width;
            unsigned int blk_color;
            bool** blkspc;
        unsigned int genColor(unsigned char red,unsigned char green,unsigned char blue );
        void spanshape(int type);
    };

};
#endif
