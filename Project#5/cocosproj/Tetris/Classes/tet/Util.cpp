#include <iostream>
#include <cstdlib> 
#include <ctime>
#include "Util.h"
namespace Tetris{
    namespace Util{
         int randInt(int st,int ed){
            if(st>ed)return randInt(ed,st);
            srand(time(NULL));
            return ((rand())%(ed-st))+st;
        }
    }
}

