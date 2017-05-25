#ifndef __TETUTIL_H_INC_
#define __TETUTIL_H_INC_

#include <iostream>
#include <cstdlib> 
#include <ctime> 
static int srandreq;

//namespace Tetris{namespace Util{
class TetrisUtil{
public:
            static int randInt(int st,int ed){
            if(st>ed)return randInt(ed,st);
            const int rndval=rand();
            const int rst =((rndval)%(ed-st+1))+st;
            //std::cout<<"randint("<<st<<","<<ed<<")val: "<<rst<<" and common rndval: "<<rndval<<std::endl;
            return rst;
        }
    
};
    //}}
#endif
