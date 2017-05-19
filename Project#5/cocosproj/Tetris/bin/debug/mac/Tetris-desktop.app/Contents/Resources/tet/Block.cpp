
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include "Block.h"
#include "Util.h"
//#include "GameUserClsDelegate.h"
//#include "User.h"
using namespace std;
using namespace Tetris;
//using namespace Tetris::Delegates;
//using namespace Tetris::Users;
namespace Tetris{
/*inline Block::Block(int type){
                this->spanshape(type);
                this->setBlockColor(this->genRandomColor());
            }
            
inline ~Block::Block(){
                if(this->blkspc!=NULL){
                    for(int i=0;i<this->blkary_height;i++){
                        delete [] this->blkspc[i];
                    }
                    delete [] this->blkspc;
                }
            }
Block::Block(int type,unsigned int color){
                this->spanshape(type);
                this->setBlockColor(color);
            }*/
void Block::setBlockColor(unsigned int clr){
                this->blk_color=clr;
            }
unsigned int Block::getBlockColorAsRaw(){
                return this->blk_color;
            }
unsigned int Block::genRandomColor(){
                return this->genSampleColor((unsigned char)Tetris::Util::randInt(1,7));
            }
unsigned int Block::genSampleColor(const unsigned char samplenum){
                switch(samplenum){
                    case 2:{//pink600
                        return this->genColor(0xd8,0x1b,0x60);
                    }
                    case 3:{//purple700
                        return this->genColor(0x7b,0x1f,0xa2);
                    }
                    case 4:{//deeppurpleA400
                        return this->genColor(0x65,0x1f,0xff);
                    }
                    case 5:{//teal600
                        return this->genColor(0x00,0x89,0x7b);
                    }
                    case 6:{//red500
                        return this->genColor(0xf4,0x43,0x36);
                    }
                    case 7:{//red900
                        return this->genColor(0xb7,0xc1,0xc1);
                    }
                    case 1:
                    default:return this->genColor(0x30,0x3f,0x9f);
                }

            }

string Block::getBlockColorAsHexString(){
                //string rst = "";
                stringstream ss;
                unsigned int clr = this->blk_color;
                ss.fill('0');
                for(int i=4-2;i>=0;i--){
                    unsigned int inters = (~((~(unsigned int)0)<<8))<<(8*i);
                    unsigned int extract = (clr&inters)>>(8*i);
                    ss.width(2);
                    ss<<hex<<extract;
                }
                //rst<<
                return ss.str();
            }
int Block::getBlockSpaceSize(){
                return this->getBlockSpaceHeight()*this->getBlockSpaceWidth();
            }
int Block::getBlockSpaceHeight(){
                return this->blkary_height;
            }
int Block::getBlockSpaceWidth(){
                return this->blkary_width;
            }
bool** Block::getBlockData(){
                return this->blkspc;
            }
int Block::getBlockType(){
                return this->blktype;
            }
void Block::freeblockspace(bool*** spc){
                bool** blockspace = *spc;
                for(int i=0;i<blkary_height;i++){
                    delete [] blockspace[i];
                }
                delete [] blockspace;
            }
bool** Block::getRotatedSpaceData(){
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
bool Block::canRotate(const vector<bool*> mapdata,const unsigned char boardwidth, unsigned char cury,unsigned char curx ){//,GameUser* guser){
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
                        if(rotated_space[j][i]&&mapdata[i+cury][j+curx]){
                            return false;
                        }
                    }
                }
                return true;
            }
bool Block::rotate(){
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
    bool Block::roatespec(){
                return false;
            }
            
void Block::printspace(){
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

unsigned int Block::genColor(unsigned char red,unsigned char green,unsigned char blue ){
                unsigned int rst = (unsigned int)blue;
                rst|=((unsigned int)red<<(8*2));
                rst|=((unsigned int)green<<(8*1));
                return rst;
            }
void Block::spanshape(int type){
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
   // };
}


