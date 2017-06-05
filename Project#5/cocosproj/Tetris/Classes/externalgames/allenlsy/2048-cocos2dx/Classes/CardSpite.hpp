#include "cocos2d.h"

#ifndef __CardSprite_H_
#define __CardSprite_H_
#include <iostream>
using namespace std;
#define FONT_SIZE 70
USING_NS_CC;
namespace allenlsy{
namespace game2048{
class CardSprite : public cocos2d::Sprite {
public:
    static CardSprite* createCardSprite(int numbers, int width, int height, float CardSpriteX, float CardSpriteY){
        CardSprite*enemy = new CardSprite();
    if (enemy && enemy->init()) {
        enemy->enemyInit(numbers, width, height, CardSpriteX, CardSpriteY);

        return enemy;

    }

    CC_SAFE_DELETE(enemy); // MYNOTE: CC_SAFE_DELETE: do { delete (p); (p) = nullptr; } while(0)
    return NULL;
    }
    virtual bool init(){
        if (!Sprite::init()) {
        return false;
    }

    return true;
    }

    CREATE_FUNC(CardSprite); //MYNOTE: CREATE_FUNC:  define a create function for a specific type, such as Layer

    int getNumber(){
    return number;
}
    void setNumber(int num){
    this->number = num;

    if (number > 0) {
        labelCardNumber->setString(String::createWithFormat("%i", number)->getCString());
    } else {
        labelCardNumber->setString("");
    }

    //判断数字的大小来调整字体
    if (number >= 0) {
        labelCardNumber->setFontSize(100);
    }
    if (number >= 16) {
        labelCardNumber->setFontSize(90);
    }
    if(number >= 128){
        labelCardNumber->setFontSize(60);
    }
    if(number >= 1024){
        labelCardNumber->setFontSize(40);
    }

        labelCardNumber->setScale(0.5f, 0.5f);
    //判断数字的大小来调整颜色
        //cout<<"start coloring"<<endl;
    //if(number == 0){
    //    layerColorBG->setColor(cocos2d::Color3B(200,190,180));
   // }
        //cout<<"finish coloring"<<endl;
//    if (number == 2) {
//        layerColorBG->setColor(cocos2d::Color3B(240,230,220));
//    }
//    if (number == 4) {
//        layerColorBG->setColor(cocos2d::Color3B(240,220,200));
 //   }
//    if (number == 8) {
//        layerColorBG->setColor(cocos2d::Color3B(240,180,120));
//    }
//    if (number == 16) {
//        layerColorBG->setColor(cocos2d::Color3B(240,140,90));
//    }
//    if (number == 32) {
//        layerColorBG->setColor(cocos2d::Color3B(240,120,90));
//    }
//    if (number == 64) {
//        layerColorBG->setColor(cocos2d::Color3B(240,90,60));
//    }
//    if (number == 128) {
//        layerColorBG->setColor(cocos2d::Color3B(240,90,60));
//    }
//    if (number == 256) {
//        layerColorBG->setColor(cocos2d::Color3B(240,200,70));
//    }
//    if (number == 512) {
//        layerColorBG->setColor(cocos2d::Color3B(240,200,70));
//    }
//    if (number == 1024) {
//        layerColorBG->setColor(cocos2d::Color3B(0,130,0));
//    }
//    if (number == 2048) {
//        layerColorBG->setColor(cocos2d::Color3B(0,130,0));
//    }
}

private:
    int number;
    void enemyInit(int numbers, int width, int height, float CardSpriteX, float CardSpriteY){
        number = numbers;

    //加入游戏的背景颜色
    auto layerColorBG = LayerColor::create(Color4B(200, 190, 180, 255), width - 15, height - 15);
    layerColorBG->setPosition(CardSpriteX, CardSpriteY);

    //判断如果大于0就显示，否则显示空
    if (numbers > 0)
    {
        //加入中间字体
        labelCardNumber = LabelTTF::create(String::createWithFormat("%i", numbers)->getCString(), "fonts/arial.ttf", FONT_SIZE);
        labelCardNumber->setPosition(layerColorBG->getContentSize().width/2, layerColorBG->getContentSize().height/2);
        labelCardNumber->setTag(8);
        layerColorBG->addChild(labelCardNumber);
    } else {
        labelCardNumber = LabelTTF::create("", "fonts/arial.ttf", FONT_SIZE);
        labelCardNumber->setPosition(layerColorBG->getContentSize().width/2, layerColorBG->getContentSize().height/2);
        labelCardNumber->setTag(8);
        layerColorBG->addChild(labelCardNumber);
    }

    this->addChild(layerColorBG);
    }

    cocos2d::LabelTTF* labelCardNumber=NULL;

    cocos2d::LayerColor* layerColorBG=NULL;


};
}}
#endif //__CardSprite_H_
