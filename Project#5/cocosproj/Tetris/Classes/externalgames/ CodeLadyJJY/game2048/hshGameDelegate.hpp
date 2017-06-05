#ifndef _hsh_2048_game_DELEGATE_H_INC_
#define _hsh_2048_game_DELEGATE_H_INC_
#include <cstdlib>
#include <cmath>
#include "../../../tet2/Util.hpp"
namespace hsh{
    namespace  CodeLadyJJY{
    namespace game2048{
        class SceneDelegate{
            public:
                SceneDelegate(){
                    this->initCls();
                }
                static SceneDelegate* getInstance(){
                    static SceneDelegate ins;
                    return &ins;
                }
                static unsigned short getGoal(unsigned int order){
                    if(order>11||order<1){
                        return 1024;
                    }
                    return (int)pow(2,order+3);
                }
                static unsigned short getRandomGoal(){
                    return getGoal(TetrisUtil::randInt(1, 11));
                }
                void setGoal(unsigned short register gl){
                    this->goal = gl;
                }
                unsigned short getGoal(){
                    return this->goal;
                }
                void resetCurrentNumber(){
                    current_number = 0;
                }
                void resetAllVar(){
                    this->initCls();
                }
                unsigned short getCurrentNumber(){
                    return this->current_number;
                }
                void setCurrentNumber(unsigned short val){
                    this->current_number = val;
                }
                unsigned int getInnerGameScore(){
                    return this->inner_gamescore;
                }
                void resetInnerGameScore(){
                    this->inner_gamescore = 0;
                }
                void setInnerGameScore(unsigned short register val){
                    inner_gamescore = val;
                }
                bool isSufficingGameGoal(){
                    return goal&&(current_number>=goal);
                }
                void setGiveUpState(bool state){
                    is_given_up = state;
                }
            
                bool getGiveUpState(){
                    return this->is_given_up;
                }
                bool isGivenUp(){
                    return this->getGiveUpState();
                }
            protected:
                void initCls(){
                    goal = 0;
                    current_number = 0;
                    inner_gamescore = 0;
                    is_given_up = false;
                }
                unsigned short goal = 0;
                 unsigned short current_number = 0;
                unsigned int inner_gamescore = 0;
                bool is_given_up = false;
        };
    }}
}
#endif
