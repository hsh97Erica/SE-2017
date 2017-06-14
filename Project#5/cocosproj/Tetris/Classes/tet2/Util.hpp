#ifndef __TETUTIL_H_INC_
#define __TETUTIL_H_INC_

#include <iostream>
#include <cstdlib> 
#include <ctime> 
static int srandreq;

//namespace Tetris{namespace Util{
/**
 @class TetrisUtil
 @brief 테트리스의 유용한 유틸을 담고있는 클래스
 */
class TetrisUtil{
public:
    /**
     @return 특정 범위내에서 랜덤한 숫자를 1개 리턴
     */
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
