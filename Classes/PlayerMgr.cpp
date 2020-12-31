#include "PlayerMgr.h"
#include "GameScene.h"
#include "DynamicDt.h"
CPlayerMgr::CPlayerMgr()
	:m_nLives(0),
	m_pPlayer(nullptr)
{
}

CPlayerMgr::~CPlayerMgr()
{
}

bool CPlayerMgr::init()
{
	if (!Node::init())
	{
		return false;
	}
	pCache = SpriteFrameCache::getInstance();
	//
	m_nLives = CDynamicDt::getInstance()->getLives();

	//加载帧和动画
	this->addSpriteCache();
	this->loadAnimation();
	this->scheduleUpdate();

	//
	_eventDispatcher->addCustomEventListener("rebirth", CC_CALLBACK_1(CPlayerMgr::rebirth,this));
	return true;
}

void CPlayerMgr::onEnter()
{
	Node::onEnter();
	//取得坐标
	this->addPlayer();
}

void CPlayerMgr::update(float delta)
{

}

Vector<Animate*>* CPlayerMgr::getAnimates()
{
	return VecAnimates;
}

void CPlayerMgr::addPlayer()
{
	m_pPlayer = CPlayer::create();
	if (m_pPlayer!= nullptr)
	{
		_eventDispatcher->dispatchCustomEvent("lives", &m_nLives);
		m_nLives--;
		this->addChild(m_pPlayer,0,"player");
	}
}


void CPlayerMgr::rebirth(EventCustom* pEvent)
{
	if ( m_nLives <= 0 )
	{
		CGameScene::getInstance()->over();
	}
	else 
	{
		this->scheduleOnce([=](float) {this->addPlayer(); }, 0.8f, "rebirthPlayer");
	}
}


void CPlayerMgr::loadAnimation()
{
	Rect rc(0, 0, 16, 16);
	//4级
	for (int i = 0; i < 4; i++)
	{//4个方向
		string strLev = to_string(i);
		for (int j = 0; j < 4; j++)
		{
			string strDir = to_string(j);
			SpriteFrame* player1_1 = SpriteFrame::create("images/player1_tank/m" + strLev + "-" + strDir + "-1.png", rc);
			SpriteFrame* player1_2 = SpriteFrame::create("images/player1_tank/m" + strLev + "-" + strDir + "-2.png", rc);
			player1_1->getTexture()->setAliasTexParameters();
			player1_2->getTexture()->setAliasTexParameters();
			Animation* player1 = Animation::createWithSpriteFrames({ player1_1,player1_2 }, 0.05f);

			pCache->addSpriteFrame(player1_1, "player1_" + strDir + "_" + strLev);

			VecAnimates[j].pushBack(Animate::create(player1));
		}
	}
}



void CPlayerMgr::addSpriteCache()
{
	char szName[32] = {};
	// 坦克爆炸帧动画
	for (int i = 0; i < 5; i++)
	{
		sprintf_s(szName, "images/blast/%d.png", i);
		SpriteFrame* pFrame = Sprite::create(szName)->getSpriteFrame();
		pFrame->getTexture()->setAliasTexParameters();
		pCache->addSpriteFrame(pFrame, "blast_" + to_string(i));
	}

	// 坦克出生前的星星帧动画
	for (int i = 0; i < 4; i++)
	{
		sprintf_s(szName, "images/star%d.png", i);
		SpriteFrame* pFrame = Sprite::create(szName)->getSpriteFrame();
		pFrame->getTexture()->setAliasTexParameters();
		pCache->addSpriteFrame(pFrame, "star_" + to_string(i));
	}

	// 坦克保护环帧动画
	for (int i = 0; i < 2; i++)
	{
		sprintf_s(szName, "images/ring%d.png", i);
		SpriteFrame* pFrame = Sprite::create(szName)->getSpriteFrame();
		pFrame->getTexture()->setAliasTexParameters();
		pCache->addSpriteFrame(pFrame, "ring_" + to_string(i));
	}
}

