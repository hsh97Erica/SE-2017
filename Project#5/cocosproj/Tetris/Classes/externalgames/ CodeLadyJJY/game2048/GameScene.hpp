#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Card.hpp"
//#include "../../../tet2/GameController.hpp"
#include "hshGameDelegate.hpp";
#include <cmath>
#define MAX_2048_LINE_COUNT 4
using namespace CodeLadyJJY::game2048;
//using namespace Tetris;
using hsh::CodeLadyJJY::game2048::SceneDelegate;
namespace  CodeLadyJJY{
    namespace game2048{
//移动方向
enum class ENUM_DIR
{
	UP,
	DOWM,
	LEFT,
	RIGHT
};

class GameScene : public cocos2d::Layer
{
private:
	int m_startmove;	//记录触摸是否移动
	int m_sound_clear;	//是否播放消除音效
	int m_score;		//记录分数
	int m_x, m_y;		//记录触摸歧视位置
	int map[MAX_2048_LINE_COUNT][MAX_2048_LINE_COUNT];		//网格数组，记录数字块的编号
	ENUM_DIR dir;		//记录移动方向
	bool isMove;		//数字块是否移动
	cocos2d::LayerColor * GameColor;	//网格背景
	cocos2d::Vector<Card *> allCard;	//记录所有卡片
    unsigned short game_goal;
    unsigned short max_blk_number;
    float tileWidth;
    float tileBorderWidth;
    hsh::CodeLadyJJY::game2048::SceneDelegate* delegate;
void newTile(){
	auto card = Card::create();
	int freeCount = 16 - allCard.size();	//剩余没有数字的网格
	int n=rand()%freeCount + 1;				//在剩余网格中随机选择一个产生新块
	int r = 0;
	int c = 0;
	int flag = 0;
	int count = 0;

	//找到第n个空的网格
	for (r=0; r<4; r++)
	{
		for (c=0; c<4; c++)
		{
			if(map[r][c] == 0)
				count++;
			if(count == n)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}

	card->show(r, c);
	allCard.pushBack(card);
	map[r][c] = allCard.getIndex(card)+1;
	GameColor->addChild(card);
	freeCount--;
	
	//剩余块数为0，判断是否死亡
	if(freeCount == 0)
	{
		for(r=0; r<4; r++)
		{
			for(c=0; c<4; c++)
			{
				int numObj = allCard.at(map[r][c]-1)->m_num;

				//上
				if(r+1 < 4)
				{
					int numNow = allCard.at(map[r+1][c]-1)->m_num;
					if(numObj == numNow)
						return ;
				}

				//下
				if(r-1 >= 0)
				{
					int numNow = allCard.at(map[r-1][c]-1)->m_num;
					if(numObj == numNow)
						return ;
				}

				//左
				if(c-1 >= 0)
				{
					int numNow = allCard.at(map[r][c-1]-1)->m_num;
					if(numObj == numNow)
						return ;
				}

				//右
				if(c+1 < 4)
				{
					int numNow = allCard.at(map[r][c+1]-1)->m_num;
					if(numObj == numNow)
						return ;
				}
			}
		}

		//没有可以移动的走法
		this->showGameOver();

		return ;
	}
}	//产生新块
	void moveAllTile(){
	m_sound_clear = false;

	switch (dir)
	{
	case ENUM_DIR::UP:
		moveUp();
		break;
	case ENUM_DIR::DOWM:
		moveDown();
		break;
	case ENUM_DIR::LEFT:
		moveLeft();
		break;
	case ENUM_DIR::RIGHT:
		moveRight();
		break;
	default:
		break;
	}

	//音效
	/*if (m_sound_clear)
		SimpleAudioEngine::getInstance()->playEffect("moveClear.mp3");
	else
		SimpleAudioEngine::getInstance()->playEffect("move.wav");
	*/
	//分数变化
	auto score = (Label *)this->getChildByTag(140);
        calcScore(m_score);
    //SceneDelegate::getInstance()->setInnerGameScore(m_score);
	score->setString(StringUtils::format("%d", m_score));
}
    int checkMaxNumber(){
        int rst = 0;
        for(int i=0;i<MAX_2048_LINE_COUNT;i++){
            for(int j=0;j<MAX_2048_LINE_COUNT;j++){
                auto cd = allCard.at(map[i][j]-1);
                rst=max(rst,cd->getCurrentNumber());
            }
        }
        return rst;
    }
	void moveUp(){
	for (int col = 0; col < 4; col++)
	{
		for (int row = 2; row >= 0; row--)
		{
			if (map[row][col] > 0)
			{
				for (int i = row + 1; i<4; i++)
				{
					//移动方向上有空网格
					if (map[i][col] == 0)
					{
						this->isMove = 1;
						map[i][col] = map[i - 1][col];
						map[i - 1][col] = 0;
						allCard.at(map[i][col] - 1)->move(i, col);
					}
					//移动方向上是数字块
					else
					{
						int numObj = allCard.at(map[i][col] - 1)->m_num;
						int numNow = allCard.at(map[i - 1][col] - 1)->m_num;
                        
						//数字相同，则消除
						if (numNow == numObj)
						{
							this->isMove = 1;
							m_sound_clear = true;
							m_score += numObj * 2;
							allCard.at(map[i][col] - 1)->doubleNum();
							allCard.at(map[i - 1][col] - 1)->removeFromParent();
							int index = map[i - 1][col];
							allCard.erase(map[i - 1][col] - 1);
							for (int j = 0; j<4; j++)
							{
								for (int k = 0; k<4; k++)
								{
									if (map[j][k] > index)
										map[j][k]--;
								}
							}
							map[i - 1][col] = 0;
						}
						break;
					}
				}
			}
		}
	}
}
	void moveDown(){
	for(int col = 0; col < 4; col++)
	{
		for(int row = 1; row < 4; row++)
		{
			if(map[row][col] > 0)
			{
				for(int i = row-1; i>=0; i--)
				{
					if(map[i][col] == 0)
					{
						this->isMove = 1;
						map[i][col] = map[i+1][col];
						map[i+1][col] = 0;
						allCard.at(map[i][col]-1)->move(i, col);
					}
					else
					{
						int numObj = allCard.at(map[i][col]-1)->m_num;
						int numNow = allCard.at(map[i+1][col]-1)->m_num;

						if(numNow == numObj)
						{
							this->isMove = 1;
							m_sound_clear = true;
							m_score += numObj*2;
							allCard.at(map[i][col]-1)->doubleNum();
							allCard.at(map[i+1][col]-1)->removeFromParent();
							int index = map[i+1][col];
							allCard.erase(map[i+1][col]-1);
							for(int j=0; j<4; j++)
							{
								for(int k=0; k<4; k++)
								{
									if(map[j][k] > index)
										map[j][k]--;
								}
							}
							map[i+1][col] = 0;
						}
						break;
					}
				}
			}
		}
	}
}
	void moveRight(){
	for(int row = 0; row < 4; row++)
	{
		for(int col = 2; col >= 0; col--)
		{
			if(map[row][col] > 0)
			{
				for(int j = col+1; j<4; j++)
				{
					if(map[row][j] == 0)
					{
						this->isMove = 1;
						map[row][j] = map[row][j-1];
						map[row][j-1] = 0;
						allCard.at(map[row][j]-1)->move(row, j);
					}
					else
					{
						int numObj = allCard.at(map[row][j]-1)->m_num;
						int numNow = allCard.at(map[row][j-1]-1)->m_num;

						if(numNow == numObj)
						{
							this->isMove = 1;
							m_sound_clear = true;
							m_score += numObj*2;
							allCard.at(map[row][j]-1)->doubleNum();
							allCard.at(map[row][j-1]-1)->removeFromParent();
							int index = map[row][j-1];
							allCard.erase(map[row][j-1]-1);
							for(int i=0; i<4; i++)
							{
								for(int k=0; k<4; k++)
								{
									if(map[i][k] > index)
										map[i][k]--;
								}
							}
							map[row][j-1] = 0;
						}
						break;
					}
				}
			}
		}
	}
}
	void moveLeft(){
	for(int row = 0; row < 4; row++)
	{
		for(int col = 1; col < 4; col++)
		{
			if(map[row][col] > 0)
			{
				for(int i = col-1; i>=0; i--)
				{
					if(map[row][i] == 0)
					{
						this->isMove = 1;
						map[row][i] = map[row][i+1];
						map[row][i+1] = 0;
						allCard.at(map[row][i]-1)->move(row, i);
					}
					else
					{
						int numObj = allCard.at(map[row][i]-1)->m_num;
						int numNow = allCard.at(map[row][i+1]-1)->m_num;

						if(numNow == numObj)
						{
							this->isMove = 1;
							m_sound_clear = true;
							m_score += numObj*2;
							allCard.at(map[row][i]-1)->doubleNum();
							allCard.at(map[row][i+1]-1)->removeFromParent();
							int index = map[row][i+1];
							allCard.erase(map[row][i+1]-1);
							for(int j=0; j<4; j++)
							{
								for(int k=0; k<4; k++)
								{
									if(map[j][k] > index)
										map[j][k]--;
								}
							}
							map[row][i+1] = 0;
						}
						break;
					}
				}
			}
		}
	}
}
    void calcScore(int scor){
        this->delegate->setInnerGameScore(scor);
        this->delegate->setCurrentNumber(checkMaxNumber());
    }
void showGameOver(){
	//显示GameOver
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	TTFConfig config("fonts/Marker Felt.ttf", winSize.width / 8);
        
	auto label = Label::createWithTTF(config, "GAME OVER");
        label->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	//label->setPosition(Vec2(winSize_width / 2, winSize_height / 2));
	this->addChild(label);
    
}

public:
	static cocos2d::Scene * createScene(){
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
    }
    
    void setGameGoal(unsigned short gmgoal){
        game_goal = gmgoal;
    }
    
    unsigned short getGameGoal(){
        return game_goal;
    }
    unsigned short getCurrentBlockNumber(){
        return max_blk_number;
    }
    bool checkSufficingGameGoal(){
        return getCurrentBlockNumber()>=getGameGoal();
    }
    void setSceneDelegateCls(hsh::CodeLadyJJY::game2048::SceneDelegate* scdel){
        this->delegate = scdel;
    }
virtual bool init(){
	if(!Layer::init())
		return false;
    tileBorderWidth = 8;
    //max_blk_number =game_goal= 0;
	this->isMove = 0;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    tileWidth = (winSize.height-tileBorderWidth*5)/4;
	//背景色
	auto BGColor = LayerColor::create(Color4B(211, 211, 211, 255));
	this->addChild(BGColor);
    TTFConfig config("fonts/Marker Felt.ttf", winSize.width / 8);
	//分数标题
    auto scoreTitle = Label::createWithTTF("SCORE", "fonts/Marker Felt.ttf", (winSize.width*4/18)/5); //Label::createWithSystemFont("SCORE", "Consolas", 70);
    scoreTitle->setPosition(Vec2(winSize.width * 15 / 18, winSize.height * 3 / 4));
    Color3B scoreColor(255,255,255); //142, 142, 142);
	scoreTitle->setColor(scoreColor);
	addChild(scoreTitle,1);

	//分数
	m_score = 0;
	auto scoreNum = Label::createWithTTF("0", "fonts/Marker Felt.ttf", (winSize.width*4/18)/6); //Label::createWithSystemFont("0", "Consolas", 60);
	scoreNum->setTag(140);
	scoreNum->setPosition(Vec2(winSize.width * 15 / 18, winSize.height / 2));
	scoreNum->setColor(scoreColor);
	addChild(scoreNum,1);

	//添加New Game按钮
	//auto menuItemNew = MenuItemFont::create("New Game", CC_CALLBACK_1(GameScene::menuCallBack, this));
	auto menuItemNew = MenuItemFont::create("Give up Game", CC_CALLBACK_1(GameScene::menuCallBack2, this));
    auto menu = Menu::create(menuItemNew, NULL);

	menu->setPosition(Vec2::ZERO);
	menuItemNew->setPosition(Vec2(winSize.width * 15 / 18, winSize.height / 8));
	menuItemNew->setColor(Color3B(0, 0, 0));

	this->addChild(menu);
    float tileMapWidth = winSize.height;
	//添加网格背景
	GameColor = LayerColor::create(Color4B(142, 142, 142, 255), tileMapWidth, tileMapWidth);
	GameColor->setPosition(Vec2::ZERO);
	this->addChild(GameColor);

	//添加网格的每一小块
	for(int row=0; row<4; row++)
	{
		for(int col=0; col<4; col++)
		{
			auto tiled = LayerColor::create(Color4B(191, 191, 191, 255), tileWidth, tileWidth);
			tiled->setPosition(Vec2(col*tileWidth+(col+1)*tileBorderWidth, 
				row*tileWidth+(row+1)*tileBorderWidth));
			GameColor->addChild(tiled);
		}
	}

	//初始化逻辑网格数组
	for(int row=0; row<4; row++)
	{
		for(int col=0; col<4; col++)
		{
			map[row][col] = 0;
		}
	}

	//初始化数字块
	newTile();

	//触摸侦听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);//跟随鼠标移动

	listener->onTouchBegan = [&](Touch * t, Event * e){//记录鼠标起始位置
		m_x = t->getLocation().x;
		m_y = t->getLocation().y;
		m_startmove = true;
		return true;
	};

	listener->onTouchMoved = [&](Touch * t, Event * e){
		int x = t->getLocation().x;
		int y = t->getLocation().y;

		//判断移动方向
		if(m_startmove && (abs(m_x - x) > 10 || abs(m_y - y) > 10))
		{
			m_startmove = false;
			if(abs(m_x - x) > abs(m_y - y))
			{
				if(m_x < x)
					dir = ENUM_DIR::RIGHT;
				else
					dir = ENUM_DIR::LEFT;
			}
			else
			{
				if (m_y < y)
					dir = ENUM_DIR::UP;
				else
					dir = ENUM_DIR::DOWM;
			}
			//移动所有块
			moveAllTile();
			//产生新块
			if (isMove)
			{
				newTile();
				isMove = 0;
			}	
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//键盘事件侦听
	auto keyboardListener = EventListenerKeyboard::create();

	keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e){//判断移动方向并改变方向
		m_sound_clear = false;

		switch (k)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			moveLeft();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			moveRight();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			moveUp();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			moveDown();
			break;
		default:
			break;
		}

		//音效
		//if (m_sound_clear)SimpleAudioEngine::getInstance()->playEffect("moveClear.mp3");

		//分数变化
		auto score = (Label *)this->getChildByTag(140);
		score->setString(StringUtils::format("%d", m_score));
        calcScore (m_score);
		if (isMove)
		{
			newTile();
			//SimpleAudioEngine::getInstance()->playEffect("move.wav");
			isMove = 0;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}

	void menuCallBack(cocos2d::Ref* obj){
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
    void menuCallBack2(cocos2d::Ref* obj){
        delegate->setGiveUpState(true);
        std::cout<<"push give up btn and call function "<< (delegate->getGiveUpState())<<std::endl;
    }
	CREATE_FUNC(GameScene);
};
    }}
#endif
