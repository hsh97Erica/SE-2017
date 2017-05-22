#ifndef __TETUTIL_H_INC_
#define __TETUTIL_H_INC_

#include <iostream>
#include <cstdlib> 
#include <ctime> 
//namespace Tetris{namespace Util{
class TetrisUtil{
public:
        static int randInt(int st,int ed){
            if(st>ed)return randInt(ed,st);
            srand(time(NULL));
            return ((rand())%(ed-st+1))+st;
        }
};
    //}}
#endif
