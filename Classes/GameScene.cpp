#include "GameScene.h"
#include "GameOver.h"
CGameScene* CGameScene::m_pInstance = nullptr;

CGameScene::CGameScene()
	:m_bDeadOnce(false)
{

}

CGameScene::~CGameScene()
{

}

CGameScene* CGameScene::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = CGameScene::create();
		return m_pInstance;
	}
	return m_pInstance;
}

bool CGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//界面数据
	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
	this->setDirtion();
	//背景灰
	LayerColor* pBgColor = LayerColor::create(Color4B(100, 100, 100, 255));
	this->addChild(pBgColor);
	//键盘事件
	this->addKeyBoardEvent();
	//地图
	m_pTiledMapMgr = CTiledMapMgr::create();
	this->addChild(m_pTiledMapMgr);
	//m_pTiledMapMgr->setIgnoreAnchorPointForPosition(false);
	m_pTiledMapMgr->setPosition(Vec2(24, 8));
	//玩家
	m_pPlayerMgr = CPlayerMgr::create();
	this->addChild(m_pPlayerMgr);
	//敌人
	m_pEnemyMgr = CEnemyMgr::create();
	this->addChild(m_pEnemyMgr);
	//右侧图标
	m_pIconMgr = CIconMgr::create();
	this->addChild(m_pIconMgr);
	//子弹
	m_pBulletMgr = CBulletMgr::create();
	this->addChild(m_pBulletMgr);
	return true;
}

void CGameScene::over()
{
	if (!m_bDeadOnce)
	{
		m_bDeadOnce = true;
		_eventDispatcher->removeAllEventListeners();

		Sprite* pOver = Sprite::create("images/gameover.png");
		pOver->getTexture()->setAliasTexParameters();
		this->addChild(pOver);

		pOver->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y - pOver->getContentSize().height / 2));

		pOver->runAction(Sequence::create(
			MoveTo::create(4, Vec2(visibleSize.width / 2, visibleSize.height / 2)),
			CallFunc::create(
				[this]()
				{
					this->destroyAndNext();
					Director::getInstance()->replaceScene(CGameOver::create());

				}),
			nullptr));

			//CallFunc::create(CC_CALLBACK_0(CGameMenu::initTankSelector, this)),
	}

}

void CGameScene::destroyAndNext()
{
	this->cleanup();
	this->removeAllChildrenWithCleanup(true);
	m_pInstance = nullptr;
}

void CGameScene::addKeyBoardEvent()
{
	EventListenerKeyboard* pListener = EventListenerKeyboard::create();
	pListener->onKeyPressed = CC_CALLBACK_2(CGameScene::onKeyPressed, this);
	pListener->onKeyReleased = CC_CALLBACK_2(CGameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

void CGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_D:
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pPlayerMgr->getChildByName("player"));
		if (pPlayer)
		{
			pPlayer->setDir(table[keyCode]);
			if (pPlayer->getIsShoot())
			{
				pPlayer->startMove();
				pPlayer->startAnimate();
			}
		}
		break;
	}
	default:
		break;
	}
	if (EventKeyboard::KeyCode::KEY_J == keyCode)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pPlayerMgr->getChildByName("player"));
		pPlayer->fire();
	}
}

void CGameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_D:
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pPlayerMgr->getChildByName("player"));
		if (pPlayer)
		{
			if (pPlayer->getIsMove())
			{
				pPlayer->stopMove();
				pPlayer->stopAnimate();
			}
		}
		break;
	}
	default:
		break;
	}
}

void CGameScene::setDirtion()
{
	table[EventKeyboard::KeyCode::KEY_W] = Dir::E_DIR_UP;
	table[EventKeyboard::KeyCode::KEY_A] = Dir::E_DIR_LEFT;
	table[EventKeyboard::KeyCode::KEY_S] = Dir::E_DIR_DOWN;
	table[EventKeyboard::KeyCode::KEY_D] = Dir::E_DIR_RIGHT;
}
