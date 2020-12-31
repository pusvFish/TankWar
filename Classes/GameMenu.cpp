#include "GameMenu.h"
#include "LoadScene.h"
CGameMenu::CGameMenu()
	:m_nState(0)
{
}

CGameMenu::~CGameMenu()
{
}

bool CGameMenu::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//界面数据
	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	this->initBackGround();
	this->addAnimation();
	return true;
}
//1、画面下落、点击下落、space下落
//2、下落后，坦克动画
//3、关闭之前的监听，开启新监听
//4、点击跳转，enter跳转、上下小坦克按键切换


void CGameMenu::initBackGround()
{
	m_pBg = Sprite::create("images/select_player.png");
	m_pBg->getTexture()->setAliasTexParameters();//关闭抗锯齿，cocos默认压缩图片 
	m_pBg->setPosition(Vec2(visibleSize.width / 2 ,visibleSize.height / 2 * 3 ));
	//m_pBg->setPosition(Vec2(visibleSize.width / 2 ,visibleSize.height / 2  ));
	this->addChild(m_pBg);
	this->fallDownEvent();
}

void CGameMenu::fallDownEvent()
{
	m_pBg->runAction(Sequence::create(
		MoveTo::create(4, Vec2(visibleSize.width / 2, visibleSize.height / 2)),
		CallFunc::create(CC_CALLBACK_0(CGameMenu::initTankSelector,this)),
		nullptr));
	//
	EventListenerKeyboard* pKeyListener = EventListenerKeyboard::create();
	EventListenerTouchOneByOne* pTouchListener = EventListenerTouchOneByOne::create();

	pKeyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* pEvent)
	{ 
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_SPACE:
		{
			m_pBg->stopAllActions();
			m_pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
			this->initTankSelector();
			break;
		}
		default:
			break;
		}	
	};

	pTouchListener->onTouchBegan = [this](Touch* , Event* )
	{
		m_pBg->stopAllActions();
		m_pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->initTankSelector();
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(pKeyListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pTouchListener, this);
}
//
void CGameMenu::changeEvent()
{
	EventListenerKeyboard* pKeyListener = EventListenerKeyboard::create();
	EventListenerTouchOneByOne* pTouchListener = EventListenerTouchOneByOne::create();

	pKeyListener->onKeyPressed = CC_CALLBACK_2(CGameMenu::onKeyPressed,this);
	pTouchListener->onTouchBegan = CC_CALLBACK_2(CGameMenu::onTouchBegan,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(pKeyListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pTouchListener, this);
}

//开始动画动作
void CGameMenu::initTankSelector()
{
	//取消下落监听，开启切换场景监听
	_eventDispatcher->removeAllEventListeners();
	//小坦克选择器
	m_pTankSelector = Sprite::create("images/m0-2-1.png");
	m_pTankSelector->getTexture()->setAliasTexParameters();
	m_pTankSelector->setPosition(Vec2(75,92));
	m_pBg->addChild(m_pTankSelector);

	Animate* pAnima = Animate::create(m_pAnimationTank);
	m_pTankSelector->runAction(RepeatForever::create(pAnima));

	//切换场景事件
	this->changeEvent();
}
//
bool CGameMenu::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	Director::getInstance()->replaceScene(CLoadScene::create());
	return true;
}
//
void CGameMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent)
{
	Vec2 pos = m_pTankSelector->getPosition();
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ENTER:
	{
		Director::getInstance()->replaceScene(CLoadScene::create());
	}
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_W:
	{
		if ( 0 == m_nState )
		{
			m_nState = 2;
			m_pTankSelector->setPosition(pos.x, pos.y - 30);
		}
		else
		{
			m_nState--;
			m_pTankSelector->setPosition(pos.x, pos.y + 15);
		}
		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_S:
	{
		if (2 == m_nState)
		{
			m_nState = 0;
			m_pTankSelector->setPosition(pos.x, pos.y + 30);
		}
		else
		{
			m_nState++;
			m_pTankSelector->setPosition(pos.x, pos.y - 15);
		}
		break;
	}
	default:
		break;
	}
}


//动画信息
void CGameMenu::addAnimation()
{
	m_pAnimationTank = Animation::create();
	char szName[32] = {};
	for (int i = 0; i < 2; i++)
	{
		sprintf_s(szName, "images/m0-2-%d.png", i + 1);
		SpriteFrame* pFrame = SpriteFrame::create(szName, Rect(0,0,16,16));
		pFrame->getTexture()->setAliasTexParameters();//设置锯齿
		m_pAnimationTank->addSpriteFrame(pFrame);
	}
	m_pAnimationTank->setDelayPerUnit(0.01);
	AnimationCache::getInstance()->addAnimation(m_pAnimationTank, "AnimatioNtank");
}