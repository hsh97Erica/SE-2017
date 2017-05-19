
//#include "User.h"
#include "GameUserClsDelegate.h"
using namespace Tetris::Users;
    namespace Tetris{
        namespace Delegates{

                void GameUserClsDelegate::setGameUserClass(GameUser* gusr){
                        this->gu = gusr;
                    }
                GameUser* GameUserClsDelegate::getGameUserClass(){
                        return this->gu;
                    }
 
        }
    }

