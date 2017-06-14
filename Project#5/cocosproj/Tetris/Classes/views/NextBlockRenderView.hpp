/**
 @file NextBlockRenderView.hpp
 @brief
 */
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
        /**
         @class NextBlockRenderBehavior
         @brief 블럭 미리 보여주기
         */
        class NextBlockRenderBehavior:public Layer{
        public:
            CREATE_FUNC(NextBlockRenderBehavior);
            /**
             @return bool렌더링성공시 true 그외 false
             */
            bool renderNextBlock(){
                if(this->gmctl!=NULL){
                    this->removeAllChildren();
                    
                    this->initInstanceLabel();
                    Size lblsz = this->getLabelSize();
                    if(!this->initLabelAtLeastOnce){
                        initLabelAtLeastOnce = true;
                        Size contsz = this->getContentSize();
                        this->setContentSize(Size(contsz.width,contsz.height+lblsz.height));
                    }
                    Block* nxtblk = this->gmctl->getLocalUser()->getNextBlock();
                    if(nxtblk==NULL){
                        return false;
                    }
                    //cout<<"success init render"<<endl;
                    const int nxtblk_height = nxtblk->getBlockSpaceHeight();
                    const int nxtblk_width = nxtblk->getBlockSpaceWidth();
                    const int prefix_view_height = this->getLabelSize().height;
                    const int boardsz = getMaxBoardSize();
                    auto colorsz =nxtblk->getBlockColor()->getColorAsArray();
                    auto blkdt = nxtblk->getBlockData();
                    auto contentsz = this->getContentSize();
                    const unsigned long long echsz = minUll( contentsz.width/boardsz,(contentsz.height-prefix_view_height)/boardsz);
                    if(colorsz==NULL||blkdt==NULL){
                        return false;
                    }
                    const int prefixheight =(boardsz-nxtblk_height+1)/2;
                    const int prefixwidth = (boardsz-nxtblk_width+1)/2;
                    bool** newblkdt = new bool*[boardsz];
                    for(int i=0;i<boardsz;i++){
                        newblkdt[i] = new bool[boardsz];
                        memset(newblkdt[i],false,sizeof(bool)*boardsz);
                    }
                    for(int i=0;i<nxtblk_height;i++){
                        for(int j=0;j<nxtblk_width;j++){
                            if(blkdt[i][j]){
                                newblkdt[i+prefixheight][j+prefixwidth] = true;
                            }
                        }
                    }
                    for(int i=0;i<boardsz;i++){
                        for(int j=0;j<boardsz;j++){
                            bool usecolor = false;
                            if(newblkdt[i][j]){
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
            Label* getLabel(){
                return this->prevlbl;
            }
            Size getLabelSize(){
                if(this->prevlbl==NULL){
                    return Size();
                }
                else{
                    return this->prevlbl->getContentSize();
                }
            }
        protected:
            Label* prevlbl = NULL;
            /**
              @return 없음
              @warning 이름 초기화
             */
            void initInstanceLabel(){
                if(this->prevlbl!=NULL){
                    this->removeChild(prevlbl);
                    this->prevlbl = NULL;
                    initInstanceLabel();
                    
                }
                else{
                    const float fntsz = 17;
                    prevlbl = Label::createWithTTF("-Next Block-", "fonts/DXSeNB-KSCpc-EUC-H.ttf", fntsz);
                    float widv = prevlbl->getContentSize().width;
                    if(widv>this->getContentSize().width){
                        widv/=2;
                    }
                    else{
                        widv = this->getContentSize().width/2-widv/2;
                    }
                    prevlbl->setPosition(Vec2(widv,this->getContentSize().height- prevlbl->getContentSize().height/2));
                    this->addChild(prevlbl);
                }
            }
            bool initLabelAtLeastOnce = false;
        private:
            /**
             @return unsigned long long 타입의 작은 값을 리턴
             */
            unsigned long long minUll(unsigned long long x,unsigned long long y){
                return x<y?x:y;
            }
            GameController* gmctl=NULL; ///< 컨트롤러에서 다음 블럭 객체를 가져오기 위함
            Size* lblsz = NULL;
        };
    }
}


#endif
