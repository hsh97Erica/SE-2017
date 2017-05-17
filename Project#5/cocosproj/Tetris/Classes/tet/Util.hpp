#ifndef __TETUTIL_H_INC_
#define __TETUTIL_H_INC_

#include <iostream>
#include <cstdlib> 
#include <ctime> 
namespace Tetris{
    namespace Util{
        int randint(int st,int ed){
            if(st>ed)return randint(ed,st);
            srand(time(NULL));
            return ((rand())%(ed-st))+st;
        }
    }
}
#endif