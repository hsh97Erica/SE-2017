#ifndef __HELLOWORLD_SCENE_ALLENLSY_H__
#define __HELLOWORLD_SCENE_ALLENLSY_H__

#include "cocos2d.h"
#include "CardSpite.hpp"
#include <string>
using namespace allenlsy::game2048;
namespace allenlsy{
namespace game2048{
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(){
        // 'scene' is an autorelease object
        auto scene = Scene::create();
        
        // 'layer' is an autorelease object
        auto layer = HelloWorld::create();
        
        // add layer as a child to scene
        scene->addChild(layer);
        
        // return the scene
        return scene;
    }

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(){
        //////////////////////////////
        // 1. super init first
        if (!Layer::init()) {
            return false;
        }
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        //    Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this); // MYNOTE: CC_CALLBACK_2: #define CC_CALLBACK_2	(	 	__selector__, __target__, ... )
        touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);
        
        //加入游戏的背景颜色
        auto layerColorBG = cocos2d::LayerColor::create(cocos2d::Color4B(180,170,160,255));
        this->addChild(layerColorBG);
        
        createCardSprite(visibleSize);
        
        //创建2张自动生成卡片
        autoCreateCardNumber();
        autoCreateCardNumber();
        
        //在上方加入游戏的分数
        string fontloc = "fonts/arial.ttf";
        //加入“分数”label
        auto labelTTFCardNumberName = LabelTTF::create("SCORE",fontloc,40);
        labelTTFCardNumberName->setPosition(Point(visibleSize.width/3,visibleSize.height-40));
        addChild(labelTTFCardNumberName);
        
        //加入具体的分数
        labelTTFCardNumber = LabelTTF::create("0",fontloc,40);
        labelTTFCardNumber->setPosition(Point(visibleSize.width-400,visibleSize.height-50));
        addChild(labelTTFCardNumber);
        
        return true;

    }

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);


    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
        Vec2 touchPoint = touch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）
        touch->getLocationInView(); // MYNOTE: getLocationInView()
        firstX = (int) touchPoint.x;
        firstY = (int) touchPoint.y;
        return true;
    }

    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event){
        Vec2 touchPoint = touch->getLocation();
        endX = firstX - touchPoint.x;
        endY = firstY - touchPoint.y;
        
        if (abs(endX) > abs(endY)) {
            if (endX + 5 > 0) {
                doLeft();
            }
            else
            {
                doRight();
            }
        } else {
            if (endY +5 > 0) {
                doDown();
            }    else {
                doUp();
            }
        }
        
        doCheckGameOver();
        //    autoCreateCardNumber();
    }

    bool doUp(){
        log("doUp");
        bool isdo = false;
        for (int x = 0; x < 4; x++) {
            for (int y = 3; y >= 0; y--) {
                
                for (int y1 = y - 1; y1 >= 0; y1--) {
                    if (cards[x][y1]->getNumber() > 0) {
                        if (cards[x][y]->getNumber() <= 0) {
                            cards[x][y]->setNumber(cards[x][y1]->getNumber());
                            cards[x][y1]->setNumber(0);
                            
                            y++;
                            isdo = true;
                        }else if(cards[x][y]->getNumber() == cards[x][y1]->getNumber()){
                            cards[x][y]->setNumber(cards[x][y]->getNumber()*2);
                            cards[x][y1]->setNumber(0);
                            
                            //改变分数
                            score += cards[x][y]->getNumber();
                            labelTTFCardNumber->setString(String::createWithFormat("%i",score)->getCString());
                            isdo = true;
                        }
                        break;
                    }
                }
                
            }
        }
        return isdo;
    }
    bool doDown(){
        log("doDown");
        bool isdo = false;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                
                for (int y1 = y + 1; y1 < 4; y1++) {
                    if (cards[x][y1]->getNumber() > 0) {
                        if (cards[x][y]->getNumber() <= 0) {
                            cards[x][y]->setNumber(cards[x][y1]->getNumber());
                            cards[x][y1]->setNumber(0);
                            
                            y--;
                            isdo = true;
                        }else if(cards[x][y]->getNumber() == cards[x][y1]->getNumber()){
                            cards[x][y]->setNumber(cards[x][y]->getNumber()*2);
                            cards[x][y1]->setNumber(0);
                            
                            //改变分数
                            score += cards[x][y]->getNumber();
                            labelTTFCardNumber->setString(String::createWithFormat("%i",score)->getCString());
                            isdo = true;
                        }
                        break;
                    }
                }
                
            }
        }
        return isdo;
    }
    bool doLeft(){
        log("doLeft");
        
        bool isDo = false;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                // 如果右边的数等于左边的数，那么左边的数乘以2，然后把右边的数设置为0，也就变为了空。
                for (int x1 = x + 1; x1 < 4; x1++) {
                    if (cards[x1][y]->getNumber() > 0) {
                        if (cards[x][y]->getNumber() <= 0) {
                            cards[x][y]->setNumber(cards[x1][y]->getNumber());
                            cards[x1][y]->setNumber(0);
                            
                            x--;
                            isDo = true;
                        }else if(cards[x][y]->getNumber() == cards[x1][y]->getNumber()){
                            cards[x][y]->setNumber(cards[x][y]->getNumber()*2);
                            cards[x1][y]->setNumber(0);
                            
                            //改变分数
                            score += cards[x][y]->getNumber();
                            labelTTFCardNumber->setString(String::createWithFormat("%i",score)->getCString());
                            isDo = true;
                        }
                        break;
                    }
                }
            }
        }
        return isDo;
    }
    bool doRight(){
        log("doRight");
        bool isdo = false;
        for (int y = 0; y < 4; y++) {
            for (int x = 3; x >= 0; x--) {
                
                for (int x1 = x - 1; x1 >= 0; x1--) {
                    if (cards[x1][y]->getNumber() > 0) {
                        if (cards[x][y]->getNumber() <= 0) {
                            cards[x][y]->setNumber(cards[x1][y]->getNumber());
                            cards[x1][y]->setNumber(0);
                            
                            x++;
                            isdo = true;
                        }else if(cards[x][y]->getNumber() == cards[x1][y]->getNumber()){
                            cards[x][y]->setNumber(cards[x][y]->getNumber()*2);
                            cards[x1][y]->setNumber(0);
                            
                            //改变分数
                            score += cards[x][y]->getNumber();
                            labelTTFCardNumber->setString(String::createWithFormat("%i",score)->getCString());
                            isdo = true;
                        }
                        break;
                    }
                }
                
            }
        }
        return isdo;
    }

    void autoCreateCardNumber(){
        
        int i, j;
        
        //判断是否这个位置已存在卡片
        do {
            i = CCRANDOM_0_1()*4;
            j = CCRANDOM_0_1()*4;
        }
        while (cards[i][j]->getNumber() > 0);
        
        cards[i][j]->setNumber(CCRANDOM_0_1()*10 < 1 ? 2: 4);
        log("(%d,%d): %d", i, j, cards[i][j]->getNumber());
        
    }

    void createCardSprite(cocos2d::Size size){
        //求出单元格的宽度和高度
        int unitSize = (size.height-100)/4;
        
        //4*4的单元格
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                CardSprite* card = CardSprite::createCardSprite(0, unitSize, unitSize, unitSize*i+140, unitSize*j+20);
                cards[i][j] = card;
                addChild(card);
            }
        }
    }
    void doCheckGameOver(){
        bool isGameOver = true;
        
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (cards[x][y]->getNumber() == 0||
                    (x>0&&(cards[x][y]->getNumber() == cards[x-1][y]->getNumber()))||
                    (x<3&&(cards[x][y]->getNumber() == cards[x+1][y]->getNumber()))||
                    (y>0&&(cards[x][y]->getNumber() == cards[x][y-1]->getNumber()))||
                    (y<3&&(cards[x][y]->getNumber() == cards[x][y+1]->getNumber()))) {
                    isGameOver = false;
                }
            }
        }
        
        if (isGameOver) {
            //游戏结束，重新开始游戏
            log("游戏结束");
            Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
        } else {
            autoCreateCardNumber();
        }
    }

    //游戏总分
    int score;

    //定义显示分数的控件
    cocos2d::LabelTTF* labelTTFCardNumber = NULL;

private:
    int firstX, firstY, endX, endY;
    CardSprite* cards[4][4];
};
}}
#endif // __HELLOWORLD_SCENE_H__
