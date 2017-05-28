#ifndef __NXTBLK_RENDERVIEW_H__INC__
#define __NXTBLK_RENDERVIEW_H__INC__

#include <iostream>
#include "cocos2d.h"
#include "../tet2/GameController.hpp"
#include "../tet2/Block.hpp"
using namespace Tetris;
using namespace cocos2d;
using namespace std;
#define BOARD_MAX_PIXEL_SIZE 4
namespace Tetris{
    namespace Views{
        class NextBlockRenderBehavior:public Layer{
        public:
            CREATE_FUNC(NextBlockRenderBehavior);
            bool renderNextBlock(){
                if(this->gmctl!=NULL){
                    this->removeAllChildren();
                    Block* nxtblk = this->gmctl->getLocalUser()->getNextBlock();
                    if(nxtblk==NULL){
                        return false;
                    }
                    const int nxtblk_height = nxtblk->getBlockSpaceHeight();
                    const int nxtblk_width = nxtblk->getBlockSpaceWidth();
                    const int boardsz = getMaxBoardSize();
                    auto colorsz =nxtblk->getBlockColor()->getColorAsArray();
                    auto blkdt = nxtblk->getBlockData();
                    auto contentsz = this->getContentSize();
                    const unsigned long long echsz = minUll( contentsz.width/boardsz,contentsz.height/boardsz);
                    if(colorsz==NULL||blkdt==NULL){
                        return false;
                    }
                    for(int i=0;i<boardsz;i++){
                        for(int j=0;j<boardsz;j++){
                            bool usecolor = false;
                            if(i<nxtblk_height&&j<nxtblk_width&&blkdt[i][j]){
                                usecolor = true;
                            }
                            auto rectNode = DrawNode::create();
                            
                            Vec2 rect[4];
                            rect[0] = Vec2(echsz*j,echsz*i);
                            rect[1] = Vec2(echsz*(j+1),echsz*i);
                            rect[2] = Vec2(echsz*(j+1),echsz*(i+1));
                            rect[3] = Vec2(echsz*j,echsz*(i+1));
                            Color4F white(1,1,1,1);
                            Color4F transp(0,0,0,0);
                            Color4F clr(colorsz[1]/255.0f,colorsz[2]/255.0f,colorsz[3]/255.0f,colorsz[0]/255.0f);
                            
                            rectNode->drawPolygon(rect, 4,usecolor? clr: transp, 1, white);
                            this->addChild(rectNode);
                        }
                    }
                    
                    return true;
                }
                else{
                    return false;
                }
            }
            void checkInstance(){
                cout<<"gugugugu... wawowawo!"<<endl;
            }
            void setGameController(GameController* gc){
                this->gmctl = gc;
            }
            int getMaxBoardSize(){
                return BOARD_MAX_PIXEL_SIZE;
            }
            
        private:
            unsigned long long minUll(unsigned long long x,unsigned long long y){
                return x<y?x:y;
            }
            GameController* gmctl=NULL;
            Size* lblsz = NULL;
        };
    }
}


#endif
