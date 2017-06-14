#include <iostream>
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "tet2/GameController.hpp"
#include "views/NextBlockRenderView.hpp"
#include "myscenes/SceneManagement.hpp"
#include "externalgames/ CodeLadyJJY/game2048/hshGameDelegate.hpp"
#include <cmath>
#include "cocosclses/AudioEnabler.h"
USING_NS_CC;
using namespace cocos2d;
using namespace std;
using namespace Tetris;
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif
using ExtGame2048Delegate = hsh::CodeLadyJJY::game2048::SceneDelegate;
using KeyCode = cocos2d::EventKeyboard::KeyCode;
using namespace Tetris::Views;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}
void HelloWorld::makeField(){
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if(this->nxtblkrenderingarea==NULL){
        this->nxtblkrenderingarea = Layer::create();
    }
    const int hei = (int)gc->getGameHeight();
    const int wid = (int)gc->getGameWidth();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    const float gapratio = 0.05f;
    const int echhei = (int)(((1.0f -gapratio)*(float)visibleSize.height)/hei);
    const int echwid =  (int)(((1.0f -gapratio)*(float)visibleSize.width)/wid);
    const int echsz = min(echhei,echwid);
    const int gapwid = visibleSize.width-echsz*wid;
    const int gaphei = visibleSize.height-echsz*hei;
    Layer* lyr = Layer::create();
    lyr->setPosition(Vec2(gapwid/2,gaphei/2));
    lyr->setContentSize(Size(echsz*wid, echsz*hei));
    if(this->nxtblkrv==NULL){
        this->nxtblkrv = NextBlockRenderBehavior::create();
    }
    this->nxtblkrv->setContentSize(Size(echsz*this->nxtblkrv->getMaxBoardSize(),echsz*this->nxtblkrv->getMaxBoardSize()));
    const int gapwid2 =visibleSize.width-(gapwid/2+echsz*wid+this->nxtblkrv->getContentSize().width);
    this->nxtblkrv->setPosition(Vec2(gapwid/2+echsz*wid+gapwid2/2,gaphei/2));
  //  this->nxtblkrenderingarea->setPosition(Vec2(0,gaphei/2));//gapwid/2,gaphei/2));
    //this->nxtblkrenderingarea->setPosition(Vec2(gapwid/2+echsz*wid,gaphei/2));
   // auto lbl = Label::createWithTTF("blk render", "fonts/arial.ttf", 10);
    //lbl->setPosition(Vec2(lbl->getContentSize().width/2,lbl->getContentSize().height/2));
    //this->nxtblkrv->addChild(lbl);
    //this->nxtblkrv->checkInstance();
    //this->addChild(this->nxtblkrenderingarea);
    
   for (int i = 0; i < hei; i++) {
        for (int j = 0; j < wid; j++) {
            auto rectNode = DrawNode::create();
        
            Vec2 rect[4];
            rect[0] = Vec2(echsz*j,echsz*i);
            rect[1] = Vec2(echsz*(j+1),echsz*i);
            rect[2] = Vec2(echsz*(j+1),echsz*(i+1));
            rect[3] = Vec2(echsz*j,echsz*(i+1));
            Color4F white(1,1,1,1);
            Color4F transp(0,0,0,0);
            rectNode->drawPolygon(rect, 4, transp, 0.7f, white);
            lyr->addChild(rectNode);

        }
    }
    this->addChild(lyr);
}
EventListenerKeyboard* HelloWorld::createNewKListener(){
    auto rst = EventListenerKeyboard::create();
    rst->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    rst->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    return rst;
}
void HelloWorld::startGame(){
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HelloWorld::play),this,0,0,0.0f,false);
    cout<<"call HelloWorld::startGame()"<<endl;
}
void HelloWorld::stateloop(float dt){
    if(this->isInExtGame){
        //cout<<"in ext game "<<endl;
        this->removeKListenerForMainGame();
        if(!HelloWorld::gc->forceend&&HelloWorld::gc->isOngoing()){
            this->pause();
        }
        auto delegate = ExtGame2048Delegate::getInstance();
        auto score = delegate->getInnerGameScore();
        auto maingamescore = gc->getLocalScore();
        auto goal = delegate->getGoal();
        //cout<<"clear? "<<(delegate->isSufficingGameGoal())<<" give up? "<<(delegate->isGivenUp())<<endl;
        if(delegate->isSufficingGameGoal()){
            float compensationRatio = 0.5f;
            switch(goal){
                case 64:{
                    compensationRatio= 0.17f;
                    break;
                }
                case 128:
                case 256:{
                   compensationRatio= 0.2f;
                    break;
                }
                case 512:{
                    compensationRatio= 0.3f;
                    break;
                }
                case 1024:{
                    compensationRatio= 0.5f;
                    break;
                }
                case 2048:{
                    compensationRatio= 0.6f;
                    break;
                }
                case 4096:{
                    compensationRatio= 0.75f;
                    break;
                }
                case 8192:{
                    compensationRatio= 0.94f;
                    break;
                }
                case 16384:{
                    compensationRatio= 3.94f;
                    break;
                }
                default:{
                    compensationRatio = 0.15f;
                break;}
            }
            
            float totalscore = ((float)score*1.5f)+ (delegate->getGoal()*2) +( (float)maingamescore*compensationRatio);
            this->gc->getLocalUser()->accumulateCurrentGameScore(totalscore);
            cout<<"bonus score(with 2048): "<<totalscore<<endl;
            this->isInExtGame = false;
            addOrRemove2048GameView(true);
            if(gc->isPaused()){
                resume();
            }
        }
        else if(delegate->isGivenUp()){
            
            double minnus = pow(10,(double)((int) log10(maingamescore)));
            
            float totalscore =(((float)score*3)+(maingamescore*0.05f)+minnus);
            if((float)maingamescore<=totalscore){
                this->gc->forceSettingScore(0);
                //this->gc->getLocalUser()->setCurrentGameScore(0);
            }
            else{
                this->gc->forceSettingScore(maingamescore-(unsigned long long)totalscore);

                //this->gc->getLocalUser()->setCurrentGameScore(maingamescore-(unsigned long long)totalscore);
            }
            this->pscorelbl->setString(gc->getScoreWithFormatForLocalUser(true));
            cout<<"bonus score(with giveup): "<<(-1*totalscore)<<endl;
            this->isInExtGame = false;
            addOrRemove2048GameView(true);
            if(gc->isPaused()){
                resume();
            }
        }
        else if(delegate->getFailGameState()){
            
            this->gc->getLocalUser()->setCurrentGameScore(((float)gc->getLocalUser()->getCurrentGameScore()+ score/0.2f));
            cout<<"bonus score(with fail): "<<(score)<<endl;
            this->isInExtGame = false;
            addOrRemove2048GameView(true);
            if(gc->isPaused()){
                resume();
            }
        }
        return;
    }
    
    if(!HelloWorld::gc->forceend&&!HelloWorld::gc->isEnd()){
        
        HelloWorld::gameloop(dt);
        
        if(this->gc->haveTimeDelta()){
            this->ptimelbl->setString(gc->getPlayTimeWithFormat(true));
        }
        if(this->gc->checkRmCntDisplayRefresh()){
            this->rmlnscntlbl-> setString(gc->getRemovedLinesCountWithFormat(true));
        }
        if(this->gc->checkScoreDisplayRefresh()){
            this->pscorelbl->setString(gc->getScoreWithFormatForLocalUser(true));
        }
        if(this->gc->getLocalUser()->canLevelUp()){
            levelup(dt);
            /*unsigned long long uplv = this->gc->getLocalUser()->getDeltaLv();
            cout<<"can level up!"<<endl;
            for(int i=0;i<uplv;i++){
                CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HelloWorld::levelup),this,0,0,0.25f,false);
            }*/
        }
        cout<<"checkers work and score is "<<(this->gc->getLocalScore())<<endl;
        int extgamechecker = (int)(log10(gc->getLocalScore())/log10(2));
        //cout<<"ext g checker: "<<extgamechecker<<"  bns_g_lv: "<<bonus_game_lv<<endl;
        if(bonus_game_lv<extgamechecker){
            bonus_game_lv=extgamechecker;
            ExtGame2048Delegate::getInstance()->resetAllVar();
            ExtGame2048Delegate::getInstance()->setGoal(ExtGame2048Delegate::getRandomGoal());
            addOrRemove2048GameView(false);
            this->isInExtGame = true;
        }
    }
    else{
        this->getChildByTag(1024)->setVisible(true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
                                                                              "res/bgms/endgamebgm.mp3", true);
        if(!readSoundEnable()){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
            
        }
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(HelloWorld::mainloopfuncschedule, this);
        cout<<"game end in scene"<<endl;
    }
}
void HelloWorld::addOrRemove2048GameView(bool remove){
    if(remove){
        auto nd = this->getChildByTag(2048);
        this->removeChild(nd);
        registerKListenerForMainGame();
        if(readSoundEnable()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        }
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(effect_ext_2048gm);
    }else{
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        effect_ext_2048gm=CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/2048gamebgm.mp3",true);
        if(!readSoundEnable()){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseEffect(effect_ext_2048gm);
        }
        auto layer2048 =Cocos2dScenes::SceneInstanceManager::createGame2048Layer();
        //layer2048->setSceneDelegateCls(hsh::CodeLadyJJY::game2048::SceneDelegate::getInstance());
        layer2048->setTag(2048);
        removeKListenerForMainGame();
        auto sz = this->getContentSize();
        layer2048->setPosition(Vec2(0,0)); //sz.width/2,sz.height/2));
        layer2048->setContentSize(sz);
        this->addChild(layer2048);
        
    }
}

void HelloWorld::drawboardingui(char** board,unsigned char* blk_clr){
    if(board==NULL){
        return;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    const int hei = (int)gc->getGameHeight();
    const int wid = (int)gc->getGameWidth();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    const float gapratio = 0.05f;
    const int echhei = (int)(((1.0f -gapratio)*(float)visibleSize.height)/hei);
    const int echwid =  (int)(((1.0f -gapratio)*(float)visibleSize.width)/wid);
    const int echsz = min(echhei,echwid);
    const int gapwid = visibleSize.width-echsz*wid;
    
    const int gaphei = visibleSize.height-echsz*hei;
    Layer* newlyr = Layer::create();
    newlyr->setPosition(Vec2(gapwid/2,gaphei/2));
    newlyr->setContentSize(Size(echsz*wid, echsz*hei));
    for (int i = 0; i < hei; i++) {
        for (int j = 0; j < wid; j++) {
            if(board[i][j]=='2'||board[i][j]=='1'){
            auto rectNode = DrawNode::create();
            Vec2 rect[4];
            rect[0] = Vec2(echsz*j,echsz*i);
            rect[1] = Vec2(echsz*(j+1),echsz*i);
            rect[2] = Vec2(echsz*(j+1),echsz*(i+1));
            rect[3] = Vec2(echsz*j,echsz*(i+1));
            Color4F white(1,1,1,1);
            Color4F transp(0,0,0,0);
                Color4F lightwhite(1,1,1,0.7f);
            
                if(board[i][j]=='2'||board[i][j]==2){
                    Color4F blockcolor(((float)blk_clr[1])/255.0f,((float)blk_clr[2])/255.0f,((float)blk_clr[3])/255.0f,((float)blk_clr[0])/255.0f);
                    rectNode->drawPolygon(rect, 4, blockcolor, 0.7f, lightwhite);
                }
                else{
                    Color4F gray(0.5f,0.5f,0.5f,1);
                    rectNode->drawPolygon(rect, 4, gray, 0.7f, lightwhite);
                }
            newlyr->addChild(rectNode);
            }
        }
    }
    if(HelloWorld::overlayblockboard!=NULL){
        this->removeChild(HelloWorld::overlayblockboard,true);
        HelloWorld::overlayblockboard=NULL;
    }
    HelloWorld::overlayblockboard = newlyr;
    this->addChild(HelloWorld::overlayblockboard);
}
void HelloWorld::gameloop(float dt){
    //cout<<"call HelloWorld::gameloop()"<<endl;
    char** board = HelloWorld::gc->innergameloop();
    unsigned char* blkclr = HelloWorld::gc->getLocalUser()->getCurrentBlock()->getBlockColor()->getColorAsArray();
    HelloWorld::drawboardingui(board,blkclr);
    this->nxtblkrv->renderNextBlock();
    if(board!=NULL){
        
        for(int i=0;i<HelloWorld::gc->getGameHeight();i++){
            delete [] board[i];
        }
        delete [] board;
    }
    if(blkclr!=NULL)
    delete [] blkclr;
}
void HelloWorld::levelup(float dt){
    cout<<"call levelup"<<endl;
    if(this->gc!=NULL){
        cout<<"dlv= "<<(this->gc->getLocalUser()->getDeltaLv())<<endl;
        this->gc->getLocalUser()->levelup(this->gc->getLocalUser()->getDeltaLv());
        if(readSoundEnable()){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/lvup.mp3");
        }
        if(this->getChildByTag(700)->getChildByTag(707)!=NULL){
            ((Label*)this->getChildByTag(700)->getChildByTag(707))->setString(StringUtils::format("%llu",this->gc->getLocalUser()->getLevel()));
        }
        else{
            cout<<"this->getChildByTag(707) is null"<<endl;
        }
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->play
    }
}
void HelloWorld::play(float dt){
    cout<<"call HelloWorld::play()"<<endl;
    HelloWorld::gc->justinit();
    HelloWorld::gc->setGameStatusToOngoing();
    HelloWorld::mainloopfuncschedule =schedule_selector(HelloWorld::stateloop);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(mainloopfuncschedule,this,1.0f/(24+1),false);
    
   // this->schedule(schedule_selector(HelloWorld::gameloop), 1);
    
}
void HelloWorld::pause(){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    //removeKListenerForMainGame();
    HelloWorld::gc -> pause();
}
void HelloWorld::resume(){
    //registerKListenerForMainGame();
    if(readSoundEnable()){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    HelloWorld::gc->resume();
    
}
void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
    if(!HelloWorld::gc->forceend&&!HelloWorld::gc->isEnd()){
        if(keyCode==KeyCode::KEY_LEFT_ARROW){
            if(readSoundEnable()){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/click.mp3");
            }
            this->gc->currentusermoveleft();
            
        }
        else if(keyCode==KeyCode::KEY_RIGHT_ARROW){
            if(readSoundEnable()){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/click.mp3");
            }
            this->gc->currentusermoveright();
        }
        else if(keyCode==KeyCode::KEY_DOWN_ARROW){
            if(readSoundEnable()){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/click.mp3");
            }
            this->gc->forcedropdownOnce();
        }
    }
}
void HelloWorld::freeRelativeConnectionWhenDeleting(){
    Director::getInstance()->getEventDispatcher()->release();
    if(this->gc!=NULL){
        this->gc->justinit();
        this->gc->setGameStatusToEnd();
        //this->gc=NULL;
    }
    
}
void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
    if(keyCode==KeyCode::KEY_ESCAPE){
        this->menuVisibleToggle(true);
    }
    if(!HelloWorld::gc->forceend&&HelloWorld::gc->isOngoing()){
        if(keyCode==KeyCode::KEY_SPACE){
        //cout<<"will call currentblockrotate"<<endl;
            this->gc-> currentblockrotate();
       // cout<<"called currentblockrotate"<<endl;
        }
        else if(keyCode==KeyCode::KEY_ENTER){
            if(readSoundEnable()){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/teleportmove.wav",false,10);
            }
            this->gc->fastdropdown();
        }
        
        else if(keyCode==KeyCode::KEY_SHIFT||keyCode==KeyCode::KEY_LEFT_SHIFT||keyCode==KeyCode::KEY_RIGHT_SHIFT){
            //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/changeblkbgm.mp3");
            this->gc->switchBlock();
        }
    }
}
void HelloWorld::onExit(){
    Scene::onExit();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

}
void HelloWorld::onEnter(){
    Scene::onEnter();
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->stop
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
                                                                          "res/bgms/maingame_bgm.mp3", true);
    if(!readSoundEnable()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    cout<<"call HelloWorld::init()"<<endl;
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    HelloWorld::overlayblockboard=NULL;
    bonus_game_lv = 7;
    if(HelloWorld::gc==NULL){
        HelloWorld::gc = GameController::getInstance();
        //cout<<(int)gc->getGameHeight()<<endl;
    }
    HelloWorld::gc->justinit();
    
    //NextBlockRenderBehavior*
    this->nxtblkrv = NextBlockRenderBehavior::create();
    this->nxtblkrv->setGameController(gc);
    this->makeField();
    this->addChild(this->nxtblkrv);
    this->isInExtGame = false;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    registerKListenerForMainGame();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
                                                                          "res/bgms/maingame_bgm.mp3", true);
    if(!readSoundEnable()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    HelloWorld::startGame();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    // add a "close" icon to exit the progress. it's an autorelease object
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    //closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
   // auto menu = Menu::create(closeItem, NULL);
    //menu->setPosition(Vec2::ZERO);
    //this->addChild(menu, 1);
    auto menuItem = MenuItemImage::create("res/images/menu_drawer.png", "res/images/menu_drawer.png", CC_CALLBACK_1(HelloWorld::menuDrawerClickCallback, this));
    menuItem->setPosition(Vec2(origin.x+menuItem->getContentSize().width/2,origin.y+visibleSize.height-menuItem->getContentSize().height/2));
     //closeItem->setPosition(Vec2(origin.x+closeItem->getContentSize().width/2,origin.y+visibleSize.height-closeItem->getContentSize().height/2));
    menuItem->setScaleX((closeItem->getContentSize().width)/(menuItem->getContentSize().width));
    menuItem->setScaleY((closeItem->getContentSize().height)/(menuItem->getContentSize().height));
    menuItem->setPosition(Vec2(origin.x+(menuItem->getContentSize().width*menuItem->getScaleX())/2,origin.y+visibleSize.height-(menuItem->getContentSize().height*menuItem->getScaleY())/2));
    auto menu = Menu::create(menuItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    Layer* infoboardlyr = Layer::create();
    const int hei = (int)gc->getGameHeight();
    const int wid = (int)gc->getGameWidth();
    const float gapratio = 0.05f;
    const int echhei = (int)(((1.0f -gapratio)*(float)visibleSize.height)/hei);
    const int echwid =  (int)(((1.0f -gapratio)*(float)visibleSize.width)/wid);
    const int echsz = min(echhei,echwid);
    const int gapwid = visibleSize.width-echsz*wid;
    const int gaphei = visibleSize.height-echsz*hei;
    infoboardlyr->setPosition(Vec2((gapwid/2+((echsz*wid)/2)),gaphei/2));
    infoboardlyr->setContentSize(Size(visibleSize.width-((echsz*wid)/2), echsz*hei));
    const float fontsz = 20;
    auto label = Label::createWithTTF("Hello World", "fonts/DXSeNB-KSCpc-EUC-H.ttf", fontsz);
    label->setAnchorPoint(Vec2(0.5f,0.5f));
    auto label2 = Label::createWithTTF("Hello World2", "fonts/DXSeNB-KSCpc-EUC-H.ttf", fontsz);
    this->rmlnscntlbl = Label::createWithTTF("Hello World3", "fonts/DXSeNB-KSCpc-EUC-H.ttf", fontsz);
    label2->setPosition(Vec2(infoboardlyr->getContentSize().width/2-label2->getContentSize().width/2,infoboardlyr->getContentSize().height-label2->getContentSize().height));
    infoboardlyr->addChild(label2);
    //cout<<"line height: "<<label2->getLineHeight()<<" , content height: "<<(label2->getBoundingBox().getMaxY()-label2->getBoundingBox().getMinY())<<endl;
    label->setPosition(Vec2(infoboardlyr->getContentSize().width/2-label->getContentSize().width/2,label2->getPosition().y-label2->getLineHeight()*1.5f-label->getContentSize().height));
    this->rmlnscntlbl->setPosition(Vec2(infoboardlyr->getContentSize().width/2-this->rmlnscntlbl->getContentSize().width/2,label->getPosition().y-label->getContentSize().height*1.5f- this->rmlnscntlbl->getContentSize().height));
    this->rmlnscntlbl-> setString(gc->getRemovedLinesCountWithFormat(true));
    this->pscorelbl = label;
    this->pscorelbl->setString(gc->getScoreWithFormatForLocalUser(true));
    infoboardlyr->addChild(label);
    infoboardlyr->addChild(this->rmlnscntlbl);
    this->ptimelbl = label2;
    this->ptimelbl->setString(gc->getPlayTimeWithFormat(true));
    auto lvlblheader = Label::createWithTTF("=LEVEL=", "fonts/arial.ttf", fontsz);
    lvlblheader->setTextColor(Color4B::WHITE);
    lvlblheader->setPosition(Vec2(infoboardlyr->getContentSize().width/2-lvlblheader->getContentSize().width/2,rmlnscntlbl->getPosition().y-lvlblheader->getContentSize().height*2));
    auto lvlbl = Label::createWithTTF("1", "fonts/arial.ttf", fontsz);
    lvlbl->setTextColor(Color4B::WHITE);
    lvlbl->setString(StringUtils::format("%llu",this->gc->getLocalUser()->getLevel()));
    lvlbl->setPosition(Vec2(infoboardlyr->getContentSize().width/2-lvlbl->getContentSize().width/2,lvlblheader->getPosition().y-lvlbl->getContentSize().height*2));
    lvlbl->setTag(707);
    infoboardlyr->addChild(lvlblheader);
    infoboardlyr->addChild(lvlbl);
    infoboardlyr->setTag(700);
    //this->ptimelbl = label2;
    this->addChild(infoboardlyr);
    // position the label on the center of the screen
    //label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
    // add the label as a child to this layer
    //this->addChild(label, 1);
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    auto overgamelayer = Layer::create();
    auto gmoverlabel = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", visibleSize.width/8);
    auto gmovermenu = this->generateGameOverMenu();
    gmoverlabel->setPosition(Vec2());
    //gmovermenu->setPosition(Vec2(max(gmovermenu->getContentSize().width/2,gmoverlabel->getContentSize().width/2),gmovermenu->getContentSize().height/2));
    overgamelayer->addChild(gmovermenu);
    gmoverlabel->setColor(Color3B::WHITE);
    gmoverlabel->setPosition(Vec2(max(gmovermenu->getContentSize().width/2,gmovermenu->getContentSize().width/2),gmovermenu->getContentSize().height/2+gmoverlabel->getContentSize().height/2));
    overgamelayer->addChild(gmoverlabel);
   // gmoverlabel->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
    overgamelayer->setPosition(Vec2());
   // overgamelayer->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
    //gmoverlabel->setTag(1024);
    overgamelayer->setTag(1024);
    //this->addChild(gmoverlabel);
    this->addChild(overgamelayer,2);
    overgamelayer->setVisible(false);
   // gmoverlabel->setVisible(false);
    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    //cout<<"loaded ok"<<endl;
    return true;
}

void HelloWorld::menuDrawerClickCallback(Ref* pSender){
    this->menuVisibleToggle(true);
   // menuCloseCallback(pSender);
}
void HelloWorld::menuVisibleToggle(bool autopauseorresume){
    if(gc->isOngoing()&&autopauseorresume){
        this->pause();
        //gc->pause();
    }
    if(this->gameoptionmenu==NULL){
        this->gameoptionmenu =this->generateOptionMenu();
        this->addChild(this->gameoptionmenu);
    }
    else{
        if(gc->isPaused()&&autopauseorresume){
            this->resume();
            //gc->resume();
        }
        this->removeChild(this->gameoptionmenu);
        this->gameoptionmenu = NULL;
    }
}
string HelloWorld::getMenuTextSoundEnabler(bool enable){
    string result = "Sound: ";
    result.append(enable?"On":"Off");
    return result;
}

bool HelloWorld::readSoundEnable(){
    return DBManager::getInstance()->readAppSettingAsBool(DBManager::getSoundEnablerKey());
}
void HelloWorld::menuSoundEnableCallback(cocos2d::Ref* pSender){
    const bool changedstate = !readSoundEnable();
    DBManager::getInstance()->changeBoolSetting(DBManager::getSoundEnablerKey(),changedstate);
    cout<<"finish change state in sqlite"<<endl;
    sounditem->setString(getMenuTextSoundEnabler(changedstate));
    /*if(changedstate){
        if(!isInExtGame)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        else
            CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeEffect(effect_ext_2048gm);
    }
    else{
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        if(isInExtGame){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseEffect(effect_ext_2048gm);
        }
    }*/
    //((Label*)mitem->getLabel())->setString(getMenuTextSoundEnabler(changedstate));
}
Menu* HelloWorld::generateOptionMenu(){
    
    auto item_1 = MenuItemFont::create("Exit", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
     sounditem= MenuItemFont::create(getMenuTextSoundEnabler(readSoundEnable()), CC_CALLBACK_1(HelloWorld::menuSoundEnableCallback, this));
    auto item_3 = MenuItemFont::create("Close", CC_CALLBACK_1(HelloWorld::gameforceResumeMenuCallback, this));
    auto menu = Menu::create(item_1,sounditem,item_3,NULL);
    menu->alignItemsVertically();
    return menu;
}
Menu* HelloWorld::generateGameOverMenu(){
    auto item_1 = MenuItemFont::create("재시작", CC_CALLBACK_1(HelloWorld::gameRestartGameCallback, this));
    auto item_3 = MenuItemFont::create("종료", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    auto menu = Menu::create(item_1,item_3,NULL);
    menu->alignItemsHorizontally();
    return menu;
}
void HelloWorld::gameforceResumeMenuCallback(Ref* pSender){
    if(gc->isPaused()&&!isInExtGame){
        gc->resume();
    }
    if(this->gameoptionmenu!=NULL){
        this->removeChild(this->gameoptionmenu);
        this->gameoptionmenu=NULL;
    }
}

void HelloWorld::gameforcePauseMenuCallback(Ref* pSender){
    if(gc->isOngoing()){
        gc->pause();
    }
}
void HelloWorld::removeKListenerForMainGame(){
    if(this->k_listener!=NULL){
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->k_listener);
        this->k_listener = NULL;
    }
}
void HelloWorld::registerKListenerForMainGame(){
    if(this->k_listener==NULL){
        this->k_listener = this->createNewKListener();
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->k_listener, 1);
    }else{
        removeKListenerForMainGame();
        this->registerKListenerForMainGame();
    }
}
void HelloWorld::gameRestartGameCallback(Ref* pSender){
    removeKListenerForMainGame();
    auto newscene =  HelloWorld::createScene();
    Director::getInstance()->replaceScene(newscene);
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    this->removeKListenerForMainGame();
    Director::getInstance()->popScene();
    //this->freeRelativeConnectionWhenDeleting();
    //Director::getInstance()->replaceScene( Cocos2dScenes::SceneInstanceManager::createMainMenuScene());
   /* Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
    #endif
    */
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
