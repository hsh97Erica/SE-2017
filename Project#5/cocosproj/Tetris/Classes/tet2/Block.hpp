/**
 @file Block.hpp
 @brief
 */
#ifndef _BLK_H_INC_
#define _BLK_H_INC_

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include "Util.hpp"
#include "GameUserClsDelegate.hpp"
#include "User.hpp"
#include "BlockColor.hpp"
using namespace std;
//using namespace Tetris;
//using namespace Tetris::Delegates;
//using namespace Tetris::Users;
using namespace Tetris::BlockSubModules;
#define TETBLK_TYPE_I 10
#define TETBLK_TYPE_L 20
#define TETBLK_TYPE_J 1+TETBLK_TYPE_L
#define TETBLK_TYPE_M 30
#define TETBLK_TYPE_Z 40
#define TETBLK_TYPE_S 1+TETBLK_TYPE_Z
#define TETBLK_TYPE_T 50

namespace Tetris{
    /**
     @class Block
     @brief 테트리스 블럭데이터를 저장하고 불러오는 객체
     */
    class Block{
        public:
            /**
             @return constructor
             @warning type이 연속된 번호범위를 벗어나면 기본값으로 생성된다
             */
            Block(int type){
               
                this->spanshape(type);
                this->setBlockColor(this->genRandomColor());
            }
            /**
             @return 모름
             @warning 메모리 터지지 않도록 메모리 수동삭제
             */
            ~Block(){
                if(this->blkspc!=NULL){
                    for(int i=0;i<this->blkary_height;i++){
                        delete [] this->blkspc[i];
                    }
                    delete [] this->blkspc;
                }
                if(this->blkcolr!=NULL){
                    delete this->blkcolr;
                }
            }
        /**
         @return constructor
         @warning 연속된 숫자의 범위를 벗어나면 기본값으로 생성
         */
            Block(int type,unsigned int color){
                this->spanshape(type);
                this->setBlockColor(color);
            }
        /**
         @return 리턴은 없지만 블럭 색상 설정
         */
            void setBlockColor(unsigned int clr){
                if(this->blkcolr==NULL){
                    this->blkcolr = new BlockColor(clr);
                }else{
                    this->blkcolr->forceApplyColor(clr);
                }
               // this->blk_color=clr;
            }
        /**
         @return 설정된 블럭 색상을 4바이트 연속된 데이터로 리턴
         */
            unsigned int getBlockColorAsRaw(){
                if(this->blkcolr==NULL){
                    return 0;
                }else{
                    this->blkcolr->packRawColorData();
                }
                //return this->blk_color;
            }
        /**
        @return 블럭 색상 객체를 리턴
         */
        BlockColor* getBlockColor(){
            return this->blkcolr;
        }
        /**
         @return 지정된 색상 안에서 랜덤으로 색상을 리턴
         */
            unsigned int genRandomColor(){
                return BlockColor::genRandomColor(); //genSampleColor((unsigned char)TetrisUtil::randInt(1,7));
            }
        /**
         @return 블럭색상을 rgba hex로 리턴
         */
            string getBlockColorAsHexString(){
                //string rst = "";
                if(this->blkcolr==NULL){
                    return NULL;
                }
                return this->blkcolr->getBlockColorAsHexString();
            }
        /**
         @return 블럭 모양의 공간 크기를 리턴
         */
            int getBlockSpaceSize(){
                return this->getBlockSpaceHeight()*this->getBlockSpaceWidth();
            }
        /**
         @return 블럭 모양의 높이를 리턴
         */
            int getBlockSpaceHeight(){
                return this->blkary_height;
            }
        /**
         @return 블럭 모양의 너비를 리턴
         */
            int getBlockSpaceWidth(){
                return this->blkary_width;
            }
        /**
         @return 블럭 모양 메트릭스
         */
            bool** getBlockData(){
                return this->blkspc;
            }
        /**
         @return 블럭 모양 인식 번호
         */
            int getBlockType(){
                return this->blktype;
            }
            void freeblockspace(bool*** spc){
                bool** blockspace = *spc;
                for(int i=0;i<blkary_height;i++){
                    delete [] blockspace[i];
                }
                delete [] blockspace;
            }
        /**
         @return 반시계 방향으로 90도 회전된 블록 데이터를 리턴
         */
            bool** getRotatedSpaceData(){
                if(this->blktype==-1||this->blkspc==NULL||(this->blkspc!=NULL&&this->blkspc[0]==NULL)||blktype==TETBLK_TYPE_M){
                    return NULL;
                }
                bool** tmpblksp = new bool*[blkary_width];
                for(int i=0;i<blkary_width;i++){
                    tmpblksp[i] = new bool[blkary_height];
                    //memset(tmpblksp[i],false,sizeof(bool)*blkary_height);
                    for(int j=0;j<blkary_height;j++) tmpblksp[i][j] = blkspc[blkary_height-1-j][i];
                }
                return tmpblksp;
            }
        /**
         @return 게임 맵에서 현재 좌표에서 회전이 가능한지 유무
         */
            bool canRotate(const vector<bool*> mapdata,const unsigned char boardwidth, unsigned char cury,unsigned char curx ){//,GameUser* guser){
                bool** rotated_space = this->getRotatedSpaceData();
                //unsigned char curx = guser->getCurrentX();
                //unsigned char cury = guser->getCurrentY();
                if(rotated_space==NULL||!mapdata.size()){
                    return false;
                }
                if(blkary_height>blkary_width&&curx+blkary_height>boardwidth){
                   /*const int movepos = blkary_height-blkary_width;
                    int movedpos = curx-movepos;
                    bool movable = true;
                    if(movepos==0||movedpos<0){
                        return false;
                    }
                    for(int i = 0;i<blkary_height;i++){
                        for(int j=0;movedpos>=0&&j<=movepos;j++){
                            if(rotated_space[j][i]&&mapdata[cury+i][movedpos+j]){
                                movable = false;
                            }
                        }
                    }
                    if(movable){
                        guser->setCurrentX((unsigned char)movedpos);
                        curx = guser->getCurrentX();
                    }else*/
                    return false;
                }
                for(int i=0;i<blkary_height;i++){
                    for(int j=0;j<blkary_width;j++){
                        if(j+curx<boardwidth && rotated_space[j][i]&&mapdata[i+cury][j+curx]){
                            return false;
                        }
                    }
                }
                return true;
            }
        /**
         @return 회전 성공 유무
         */
            bool rotate(){
                cout<<"call rotatefunc in block"<<endl;
                if(this->blktype==-1||this->blkspc==NULL||(this->blkspc!=NULL&&this->blkspc[0]==NULL)){
                    return false;
                }
                if(blktype==TETBLK_TYPE_M){
                    return true;
                }
                bool** tmpblksp = this->getRotatedSpaceData();

                for(int i=0;i<blkary_height;i++){
                    delete [] blkspc[i];
                }
                delete [] blkspc;
                blkspc = tmpblksp;
                this->blkary_height^=this->blkary_width^=this->blkary_height^=this->blkary_width;
                return true;
            }
            bool roatespec(){
                return false;
            }
            /**
             @return 콘솔에서 모양 디버깅시 확인 함수
             */
            void printspace(){
                if(this->blktype==-1||this->blkspc==NULL){
                    cout<<"we can't show space info"<<endl;
                    return;
                }
                for(int i=0;i<this->blkary_height;i++){
                    for(int j=0;j<this->blkary_width;j++){
                        cout<<blkspc[i][j]<<' ';
                    }
                    cout<<endl;
                }
            }
        private:
            int blktype;
            int blkary_height;
            int blkary_width;
            unsigned int blk_color;
        BlockColor* blkcolr = NULL;
            bool** blkspc;
            /*unsigned int genColor(unsigned char red,unsigned char green,unsigned char blue ){
                unsigned int rst = (unsigned int)blue;
                rst|=((unsigned int)red<<(8*2));
                rst|=((unsigned int)green<<(8*1));
                return rst;
            }*/
            /**
             @return 지정된 블럭 인식번호에 의해서 함수내에서 모양 직접 설정
             */
            void spanshape(int type){
                switch(type){
                    case TETBLK_TYPE_I:{
                        this->blktype = type;
                        this->blkary_height=4;
                        this->blkary_width=1;
                        this->blkspc =  new bool*[this->blkary_height];
                        for(int i=0;i<blkary_height;i++){
                            this->blkspc[i] = new bool[this->blkary_width];
                            memset(this->blkspc[i],false,sizeof(bool)*this->blkary_width);
                            this->blkspc[i][0]=true;
                        }

                        break;
                    }
                    case TETBLK_TYPE_M:{
                        this->blktype = type;
                        this->blkary_height=2;
                        this->blkary_width=2;
                        this->blkspc =  new bool*[this->blkary_height];
                        for(int i=0;i<blkary_height;i++){
                            this->blkspc[i] = new bool[this->blkary_width];
                            memset(this->blkspc[i],false,sizeof(bool)*this->blkary_width);
                            for(int j=0;j<blkary_width;j++){
                                this->blkspc[i][j]=true;
                            }
                        }
                        break;
                    }
                    case TETBLK_TYPE_T:{
                        this->blktype = type;
                        this->blkary_height=2;
                        this->blkary_width=3;
                        this->blkspc =  new bool*[this->blkary_height];
                        for(int i=0;i<blkary_height;i++){
                            this->blkspc[i] = new bool[this->blkary_width];
                            memset(this->blkspc[i],false,sizeof(bool)*this->blkary_width);
                            if(i==0){
                                for(int j=0;j<blkary_width;j++){
                                    this->blkspc[i][j]=true;
                                }
                            }else{
                                this->blkspc[i][1]=true;
                            }
                        }

                        break;
                    }
                    case TETBLK_TYPE_L:{
                        this->blktype = type;
                        this->blkary_height=3;
                        this->blkary_width=2;
                        this->blkspc =  new bool*[this->blkary_height];
                        for(int i=0;i<blkary_height;i++){
                            this->blkspc[i] = new bool[this->blkary_width];
                            memset(this->blkspc[i],false,sizeof(bool)*this->blkary_width);
                            this->blkspc[i][0]=true;
                            if(i+1==blkary_height){
                                this->blkspc[i][1]=true;
                            }
                        }

                        break;
                    }
                    case TETBLK_TYPE_J:{
                        this->blktype = type;
                        this->blkary_height=3;
                        this->blkary_width=2;
                        this->blkspc =  new bool*[this->blkary_height];
                        for(int i=0;i<blkary_height;i++){
                            this->blkspc[i] = new bool[this->blkary_width];
                            memset(this->blkspc[i],false,sizeof(bool)*this->blkary_width);
                            this->blkspc[i][1]=true;
                            if(i+1==blkary_height){
                                this->blkspc[i][0]=true;
                            }
                        }
                        break;
                    }
                    case TETBLK_TYPE_Z:{
                        this->blktype = type;
                        this->blkary_height=2;
                        this->blkary_width=3;
                        this->blkspc =  new bool*[this->blkary_height];
                        for(int i=0;i<blkary_height;i++){
                            this->blkspc[i] = new bool[this->blkary_width];
                            memset(this->blkspc[i],false,sizeof(bool)*this->blkary_width);
                            this->blkspc[i][1]=true;
                            this->blkspc[i][i==0?0:2] = true;
                        }

                        break;
                    }
                    case TETBLK_TYPE_S:{
                        this->blktype = type;
                        this->blkary_height=2;
                        this->blkary_width=3;
                        this->blkspc =  new bool*[this->blkary_height];
                        for(int i=0;i<blkary_height;i++){
                            this->blkspc[i] = new bool[this->blkary_width];
                            memset(this->blkspc[i],false,sizeof(bool)*this->blkary_width);
                            this->blkspc[i][1]=true;
                            this->blkspc[i][i!=0?0:2] = true;
                        }
                        break;
                    }
                    //
                    default:{
                        this->blktype=-1;
                        break;
                    }
                }
            }
    };

}
#endif
