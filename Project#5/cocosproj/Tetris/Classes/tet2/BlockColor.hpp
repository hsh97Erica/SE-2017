/**
 @file BlockColor.hpp
 */
#ifndef __BLK_COLR_H__INC___
#define __BLK_COLR_H__INC___
#include <iostream>
#include <sstream>
#include "Util.hpp"
using std::string;
using std::stringstream;
using std::hex;
namespace Tetris{
    namespace BlockSubModules{
        /**
         @class BlockColor
         @brief 블럭의 색상을 관리하고 불러오는 클래스
         */
        class BlockColor{
            public:
                BlockColor( unsigned char alpha, unsigned char red,  unsigned char green, unsigned char blue ){
                    this->forceChangeColor(alpha,red,green,blue);
                }
            BlockColor(unsigned int combineddt){
                this->disassembleColorDataAndApply(combineddt);
            }
            void forceChangeColor(const unsigned char alpha,const unsigned char red, const unsigned char green,const unsigned char blue){
                this->setAlpha(alpha);
                this->setRed(red);
                this->setGreen(green);
                this->setBlue(blue);
            }
            void forceApplyColor(const unsigned int dt){
                this->disassembleColorDataAndApply(dt);
            }
            static unsigned int genRandomColor(){
                return BlockColor::genSampleColor((unsigned char)TetrisUtil::randInt(1,7));
            }
            /**
             @return 미리 지정된 색상을 4바이트 변수 데이터로 리턴
             */
            static unsigned int genSampleColor(const unsigned char samplenum){
                switch(samplenum){
                    case 2:{//pink600
                        return BlockColor::genColor(0xd8,0x1b,0x60);
                    }
                    case 3:{//purple700
                        return BlockColor::genColor(0x7b,0x1f,0xa2);
                    }
                    case 4:{//deeppurpleA400
                        return BlockColor::genColor(0x65,0x1f,0xff);
                    }
                    case 5:{//teal600
                        return BlockColor::genColor(0x00,0x89,0x7b);
                    }
                    case 6:{//red500
                        return BlockColor::genColor(0xf4,0x43,0x36);
                    }
                    case 7:{//red900
                        return BlockColor::genColor(0xb7,0xc1,0xc1);
                    }
                    case 1:
                    default:return BlockColor::genColor(0x30,0x3f,0x9f);
                }
                
            }
            /**
             @return 색상데이터를 배열형식으로 리턴
             */
            unsigned char* getColorAsArray(){
                unsigned char* rst = new unsigned char[4];
                rst[0]=this->getAlphaAsRaw();
                rst[1] = this->getRedAsRaw();
                rst[2] = this->getGreenAsRaw();
                rst[3] = this->getBlueAsRaw();
                return rst;
            }
            /**
             @return 색상데이터중 녹색 데이터를 리턴
             */
            unsigned char getGreenAsRaw(){
                return this->green;
            }
            /**
             @return 색상데이터중 파랑 데이터를 리턴
             */
            unsigned char getBlueAsRaw(){
                return this->blue;
            }
            /**
             @return 색상데이터중 삘깅 데이터를 리턴
             */
            unsigned char getRedAsRaw(){
                return this->red;
            }
            /**
             @return 색상데이터중 투명도 데이터를 리턴
             */
            unsigned char getAlphaAsRaw(){
                return this->alpha;
            }
            /**
             @return 현재 설정된 색상 데이터를 4바이트 변수로 묶어 리턴
             */
            unsigned int packRawColorData(){
                return (((unsigned int)this->getAlphaAsRaw())<<(8*3))|(((unsigned int)this->getRedAsRaw())<<(8*2))|(((unsigned int)this->getGreenAsRaw())<<(8*1))|this->getBlueAsRaw();
            }
            /**
             @return 색상 데이터를 rgba hex 형식으로 리턴
             */
            string getBlockColorAsHexString(){
                //string rst = "";
                stringstream ss;
                unsigned int clr = this->packRawColorData();
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
            protected:
            unsigned char green,blue,red,alpha;
            void setGreen(unsigned char grn){
                this->green = grn;
            }
            void setBlue(unsigned char blu){
                this->blue = blu;
            }
            void setAlpha(unsigned char aph){
                this->alpha = aph;
            }
            void setRed(unsigned char rd){
                this->red = rd;
            }
            private:
            /**
             @return 4바이트 변수에서 rgba를 분해해 배열로 리턴
             */
                unsigned char* disassembleColorData(unsigned int dt){
                    unsigned char* rst = new unsigned char[4];
                    for(int i=0;i<4;i++){
                        rst[3-i]=(((~((~(unsigned int)0)<<(8)))<<(8*i))&dt)>>(8*i);
                    }
                    return rst;
                }
            /**
             @return disassembleColorData를 가져와 즉시적용
             */
            void disassembleColorDataAndApply(unsigned int dt){
                unsigned char* disasmb= this->disassembleColorData(dt);
                if(disasmb!=NULL){
                    this->forceChangeColor(disasmb[0],disasmb[1],disasmb[2],disasmb[3]);
                    delete [] disasmb;
                }
            }
            /**
             @return 색상 데이터를 4바이트 변수로 묶어 리턴
             */
            static unsigned int genColor(unsigned char red,unsigned char green,unsigned char blue ){
                unsigned int rst = (unsigned int)blue;
                rst|=((~(unsigned int)0)<<(8*3));
                rst|=((unsigned int)red<<(8*2));
                rst|=((unsigned int)green<<(8*1));
                return rst;
            }
        };
    }
}
#endif
