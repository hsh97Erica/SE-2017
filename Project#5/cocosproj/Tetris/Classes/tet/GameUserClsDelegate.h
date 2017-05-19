#ifndef __GUDELGT_H__INC
#define __GUDELGT_H__INC
#include "User.h"
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
                void setGameUserClass(GameUser* gusr);
                GameUser* getGameUserClass();
                protected:
                    GameUser* gu = NULL;
            };
        }
    }

#endif
