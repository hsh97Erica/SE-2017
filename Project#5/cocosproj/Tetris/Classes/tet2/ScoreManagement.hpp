/**
 @file ScoreMangement.hpp
 */
#ifndef __SCORMGRMNT_H_INC__
#define __SCORMGRMNT_H_INC__

#include <iostream>
#include <cmath>
#include "Util.hpp"
namespace Tetris{
    namespace ScoreManage{
        /**
         @class ScoreManagement
         @brief 점수 정책을 관리(ScorePolicyManagement가 정확하지만 최초 목적은 점수를 관라하는 것이였음)
         */
        class ScoreManagement{
            public:
                static float getScoreComboRule(float curcombo){
                    if(curcombo<1){
                        return 0;
                    }
                    return (float)(log10(curcombo)/log10(2));
                }
                static float getAdditionalComboScore(float curcombo){
                    if(curcombo<1){
                        return 0;
                    }
                    return curcombo*(float)(log10(curcombo)/log10(TetrisUtil::randInt(4,6)));
                }
                static float getComboScore(float curcombo){
                    if(curcombo<1){
                        return 0;
                    }
                    return (curcombo* getScoreComboRule(curcombo))+ getAdditionalComboScore(curcombo);
                }
                static int getDefaultScoreWhenRemovingOneLine(){
                    return 10;
                }
                static int getScoreGapWhenRemovingOneLine(){
                    return TetrisUtil::randInt(-5,5);
                }
            static int getScoreWhenPuttingFallenDownBlock(){
                return TetrisUtil::randInt(1, 5);
            }
        };
    }
}

#endif
