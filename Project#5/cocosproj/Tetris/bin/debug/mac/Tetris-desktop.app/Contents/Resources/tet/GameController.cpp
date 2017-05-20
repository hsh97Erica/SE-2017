
#include <iostream>
#include <unistd.h>
#include <vector>
#include <stack>
#include <cstring>
#include "GameController.h"
#include "GameUserClsDelegate.h"
#include "User.h"
#include "Block.h"
using namespace std;
using namespace Tetris;
using namespace Tetris::Delegates;
namespace Tetris{
        /*static GameController* GameController::getInstance(){
                return new GameController();
            }
    GameController::GameController(){
                //this->init(NULL);
                //this->isEnd = false;
                this->justinit();
            }
    GameController::GameController(InitGameInfo* igi){
                //this->isEnd=false;
                this->setInitGameInfo(igi,false);
            }*/
    /*void GameController::setGameStatus(GameController::GameStatus mGs){
                this->gs = mGs;
            }
    GameController::GameStatus GameController::getGameStatus(){return this->gs;}
    unsigned char GameController::getGameHeight(){
                return gameHeight;
            }
    unsigned char GameController::getGameWidth(){
                return gameWidth;
            }*/
    void GameController::justinit(){
                    while(this->board.size()){
                        this->board.pop_back();
                    }
                    this->gameHeight = 20;
                    this->gameWidth = 8;
        this->forceend=false;
                    this->gs = GameStatus::UNKNOWN;
                    //this->board = new bool*[this->gameHeight];
                    for(int i=0;i<this->gameHeight;i++){
                        bool* newrow = new bool[this->gameWidth];
                        memset(newrow,false,sizeof(bool)*this->gameWidth);
                        this->board.push_back( newrow);
                    }
                    this->gplaytime = 0;
                    this->usercount=1;
                    //this->gusers = new Users::GameUser*[this->usercount];
                    for(int i=0;i<this->usercount;i++){
                        this->gusers.push_back( new Users::GameUser());
                        this->gusers[i]->setCurrentY(this->gameHeight);
                        //this->gusers[i]->setCurrentY(this->gameHeight - this->gusers[i]->getCurrentBlock()->getBlockSpaceHeight());
                        this->gusers[i]->setCurrentX((this->gameWidth/2) - (this->gusers[i]->getCurrentBlock()->getBlockSpaceWidth()/2));
                    }
            }
    bool GameController::canRemoveLine(int y){
                for(int i =0;i<this->gameWidth;i++)if(!this->board[y][i])return false;
                return true;
            }
    bool GameController::isOngoing(){
                return this->getGameStatus()==GameStatus::ONGOING;
            }
    void GameController::setGameStatusToOngoing(){
                this->setGameStatus(GameStatus::ONGOING);
            }
    void GameController::setGameStatusToEnd(){
                this->setGameStatus(GameStatus::END);
            }
    void GameController::findAndRemoveLines(){
                stack<int> s;
                for(int i=0;i<this->gameHeight;i++){
                    if(canRemoveLine(i)){
                        s.push(i);
                    }
                }
                if(s.size())removeLines(s); 
            }
    void GameController::removeLines(stack<int> s){
                const int restorecount = s.size();
                while(s.size()){
                    const int pos = s.top();
                    s.pop();
                    bool* tmp = this->board[pos];
                    this->board.erase(this->board.begin()+pos);
                    delete tmp;
                }
                for(int i=0;i<restorecount;i++){
                    bool* newrow = new bool[this->gameWidth];
                    memset(newrow,false,this->gameWidth*sizeof(bool));
                    this->board.push_back(newrow);
                }
            }
    void GameController::init(InitGameInfo* igi){
                if(igi==NULL){
                    this->justinit();
                }
            }
    void GameController::innergameloop(){
            if(!this->forceend&&this->isOngoing()){
                
                this->printcurrentboard();
                if(checkEnd()){this->setGameStatusToEnd();}
                if(!this->usercheck())return;
                Users::GameUser* guser = this->gusers[0];
                this->findAndRemoveLines();
                cout<<"rm ln ok"<<endl;
                if(this->candropdown()){
                    guser->setCurrentY(guser->getCurrentY()-1);
                    cout<<"drop if inner ok"<<endl;
                }else{
                    this->saveBlockAndCheck(guser);
                    cout<<"save block ok"<<endl;
                }
                cout<<"drop down ok"<<endl;
                gplaytime++;
                cout<<"1 loop end"<<endl;
            }
        }
    void GameController::run(){
                this->justinit();
                //this->init(NULL);
                this->play();
            }
    void GameController::setInitGameInfo(InitGameInfo* igi,bool withDelPrevMem){
                if(igi!=NULL){
                    if(withDelPrevMem&&this->initginfo!=NULL){
                        delete this->initginfo;
                    }
                    this->initginfo = igi;
                }
            }
    void GameController::currentblockrotate(){
                if(!this->usercheck())return;
                Users::GameUser* guser = this->gusers[0];
                if(guser->getCurrentBlock()->canRotate(this->board,this->gameWidth,guser->getCurrentY(),guser->getCurrentX()))
                guser->getCurrentBlock()->rotate();
            }
    void GameController::currentusermoveleft(){
                if(!this->usercheck())return;
                Users::GameUser* guser = this->gusers[0];
                if(this->canmoveleft()){
                    guser->setCurrentX(guser->getCurrentX()-1);
                }
            }
    void GameController::currentusermoveright(){
                if(!this->usercheck())return;
                Users::GameUser* guser = this->gusers[0];
                if(this->canmoveright()){
                    guser->setCurrentX(guser->getCurrentX()+1);
                }
            }
    void GameController::saveBlockAndCheck(Users::GameUser* guser){
                Block* savableblock = guser->cycleBlock();
                if(saveBlockAt(savableblock,guser->getCurrentY(),guser->getCurrentX())){
                       // guser->setCurrentY(this->gameHeight - guser->getCurrentBlock()->getBlockSpaceHeight());
                        guser->setCurrentX((this->gameWidth/2) - (guser->getCurrentBlock()->getBlockSpaceWidth()/2));
                        delete savableblock;
                        guser->setCurrentY(this->gameHeight);
                        //if(checkEnd())this->gs = GameStatus::END;
                }
                
            }
    void GameController::printposinfo(){
                cout<<"y: "<<this->gusers[0]->getCurrentY()<<"  x: "<<this->gusers[0]->getCurrentX()<<endl;
            }
    void GameController::play(){
                this->gs=GameStatus::ONGOING;
                while(!this->forceend&&!this->isEnd()){
                    while(!this->forceend&&this->gs==GameStatus::ONGOING){
                        cout<<"new loop"<<endl;
                        //this->printposinfo();

                        this->printcurrentboard();
                        cout<<"printboard ok"<<endl;
                        if(checkEnd()){this->gs = GameStatus::END;}
                        cout<<"check end ok"<<endl;
                        if(!this->usercheck())break;
                        Users::GameUser* guser = this->gusers[0];
                        this->findAndRemoveLines();
                        cout<<"rm ln ok"<<endl;
                        if(this->candropdown()){
                            guser->setCurrentY(guser->getCurrentY()-1);
                            cout<<"drop if inner ok"<<endl;
                        }else{
                            this->saveBlockAndCheck(guser);
                            cout<<"save block ok"<<endl;
                        }
                        cout<<"drop down ok"<<endl;
                        gplaytime++;
                        usleep(1000000);
                        cout<<"1 loop end"<<endl;
                    }
                    if(this->gs!=GameStatus::END){
                        usleep(1000000/2);
                    }
                }
                cout<<endl<<endl<<"program finish"<<endl;
            }
   /* void GameController::pause(){
                this->gs = GameStatus::PAUSE;
            }
    bool GameController::isPaused(){
                return this->gs==GameStatus::PAUSE;
            }
            bool isEnd(){
                return this->gs==GameStatus::END;
            }
            void resume(){
                this->gs = GameStatus::ONGOING;
            }*/
    bool GameController::checkEnd(){
                if(!this->usercheck())return true;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                Block* blk = guser->getCurrentBlock();
                const int blkhg = blk->getBlockSpaceHeight();
                const int blkwd = blk->getBlockSpaceWidth();
                bool** blddt = NULL;
                blddt= blk->getBlockData();
                for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(cury+i<this->gameHeight&&blddt[i][j]&&board[cury+i][curx+j]){
                            return true;
                        }
                    }
                }
                return false;
            }
    char** GameController::getVisualizedBoard(){
                if(this->board.size()){
                    char** printboard = new char*[this->gameHeight];
                    for(int i=0;i<this->gameHeight;i++){
                        printboard[i] = new char[this->gameWidth];
                        for(int j=0;j<this->gameWidth;j++){
                            printboard[i][j] = this->board[this->gameHeight-1-i][j]?'1':'0';
                        }
                    }
                    if(!this->usercheck())return NULL;
                    Users::GameUser* guser = this->gusers[0];
                    unsigned short cury = guser->getCurrentY();
                    unsigned short curx = guser->getCurrentX();
                    Block* curblk = guser->getCurrentBlock();
                    const int blkhg = curblk->getBlockSpaceHeight();
                    const int blkwd = curblk->getBlockSpaceWidth();
                     bool** blddt = NULL;
                    blddt= curblk->getBlockData();
                    for(int i=0;i<blkhg;i++){
                        for(int j=0;j<blkwd;j++){
                            if(i+cury<this->gameHeight&&blddt[i][j]){
                                printboard[this->gameHeight-1-(i+cury)][j+curx] = '2';
                            }
                        }
                    }
                    return printboard;
                }
                else{
                    return NULL;
                }
            }
    
    bool GameController::canmoveleft(){
                 if(!this->usercheck())return false;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                if(curx==0){
                    return false;
                }
                curx--;
                Block* curblk = guser->getCurrentBlock();
                const int blkhg = curblk->getBlockSpaceHeight();
                const int blkwd = curblk->getBlockSpaceWidth();
                 bool** blddt = NULL;
             blddt= curblk->getBlockData();
             for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(i+cury<this->gameHeight&&blddt[i][j]&&board[i+cury][curx+j]){
                            return false;
                        }
                    }
                }
                return true;
            }
            
    bool GameController::canmoveright(){
                if(!this->usercheck())return false;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                curx++;
                Block* curblk = guser->getCurrentBlock();
                const int blkhg = curblk->getBlockSpaceHeight();
                const int blkwd = curblk->getBlockSpaceWidth();
                if(curx>getGameWidth()-blkwd){
                    return false;
                }
                bool** blddt = NULL;
             blddt= curblk->getBlockData();
             for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(blddt[i][j]&&board[i+cury][curx+j]){
                            return false;
                        }
                    }
                }
                return true;
            }
    bool GameController::fastdropdown(){
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                bool opt = false;
                while(candropdown()){
                    if(!opt){
                        opt = true;
                    }
                    guser->setCurrentY(guser->getCurrentY()-1);
                }
                if(opt){
                    this->saveBlockAndCheck(guser);
                }
                return opt;
            }

    bool GameController::candropdown(){
                if(!this->usercheck())return false;
                Users::GameUser* guser = this->gusers[0];
                unsigned short cury = guser->getCurrentY();
                unsigned short curx = guser->getCurrentX();
                if(cury==0){
                    return false;
                }
                cury--;
                Block* curblk = guser->getCurrentBlock();
                const int blkhg = curblk->getBlockSpaceHeight();
                const int blkwd = curblk->getBlockSpaceWidth();
                bool** blddt = NULL;
                blddt= curblk->getBlockData();
                for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(i+cury<this->gameHeight&&blddt[i][j]&&board[i+cury][curx+j]){
                            return false;
                        }
                    }
                }
                return true;
            }

    bool GameController::usercheck(){
                if(usercount==0||!this->gusers.size()||(this->gusers.size()&&this->gusers[0]==NULL)){
                    return false;
                }
                return true;
            }
    bool GameController::saveBlockAt(Block* blk, unsigned short cury,unsigned short curx ){
                if(!this->board.size()){
                    return false;
                }
                const int blkhg = blk->getBlockSpaceHeight();
                const int blkwd = blk->getBlockSpaceWidth();
                 bool** blddt = NULL;
                 if(cury+blkhg>this->gameHeight){
                     this->gs = GameStatus::END;
                     //this->isEnd =true;
                    return false;
                 }
                blddt= blk->getBlockData();
                for(int i=0;i<blkhg;i++){
                    for(int j=0;j<blkwd;j++){
                        if(blddt[i][j]&&i+cury<this->gameHeight){
                            board[i+cury][j+curx] = true;
                        }
                    }
                }
                return true;
            }

}

