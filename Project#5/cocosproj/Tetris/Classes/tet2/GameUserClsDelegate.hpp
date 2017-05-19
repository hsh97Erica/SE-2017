#ifndef __GUDELGT_H__INC
#define __GUDELGT_H__INC
#include "User.hpp"
using namespace Tetris::Users;
    namespace Tetris{
        namespace Delegates{
            class GameUserClsDelegate{
                public:
                    GameUserClsDelegate(GameUser* gusr){
                        this->setGameUserClass(gusr);
                    }
                    ~GameUserClsDelegate(){
                        this->gu = NULL;
                    }
                    void setGameUserClass(GameUser* gusr){
                        this->gu = gusr;
                    }
                    GameUser* getGameUserClass(){
                        return this->gu;
                    }
                protected:
                    GameUser* gu = NULL;
            };
        }
    }

#endif