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
	//��������
	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	this->initBackGround();
	this->addAnimation();
	return true;
}
//1���������䡢������䡢space����
//2�������̹�˶���
//3���ر�֮ǰ�ļ����������¼���
//4�������ת��enter��ת������С̹�˰����л�


void CGameMenu::initBackGround()
{
	m_pBg = Sprite::create("images/select_player.png");
	m_pBg->getTexture()->setAliasTexParameters();//�رտ���ݣ�cocosĬ��ѹ��ͼƬ 
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

//��ʼ��������
void CGameMenu::initTankSelector()
{
	//ȡ����������������л���������
	_eventDispatcher->removeAllEventListeners();
	//С̹��ѡ����
	m_pTankSelector = Sprite::create("images/m0-2-1.png");
	m_pTankSelector->getTexture()->setAliasTexParameters();
	m_pTankSelector->setPosition(Vec2(75,92));
	m_pBg->addChild(m_pTankSelector);

	Animate* pAnima = Animate::create(m_pAnimationTank);
	m_pTankSelector->runAction(RepeatForever::create(pAnima));

	//�л������¼�
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


//������Ϣ
void CGameMenu::addAnimation()
{
	m_pAnimationTank = Animation::create();
	char szName[32] = {};
	for (int i = 0; i < 2; i++)
	{
		sprintf_s(szName, "images/m0-2-%d.png", i + 1);
		SpriteFrame* pFrame = SpriteFrame::create(szName, Rect(0,0,16,16));
		pFrame->getTexture()->setAliasTexParameters();//���þ��
		m_pAnimationTank->addSpriteFrame(pFrame);
	}
	m_pAnimationTank->setDelayPerUnit(0.01);
	AnimationCache::getInstance()->addAnimation(m_pAnimationTank, "AnimatioNtank");
}