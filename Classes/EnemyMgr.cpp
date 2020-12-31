#include "EnemyMgr.h"
#include "DynamicDt.h"
#include "GameScene.h"
CEnemyMgr::CEnemyMgr()
	:m_nCurCount(0),
	m_nEnemyCount(0)
{
}

CEnemyMgr::~CEnemyMgr()
{
}

bool CEnemyMgr::init()
{
	if (!Node::init())
	{
		return false;
	}
	//
	pCache = SpriteFrameCache::getInstance();
	this->loadAnimation();
	m_nEnemyCount = CDynamicDt::getInstance()->getEnemyCount();
	this->scheduleUpdate();

	return true;
}

void CEnemyMgr::onEnter()
{
	Node::onEnter();
	m_pTiledMap = CGameScene::getInstance()->getTiledMapMgr();
	vecEnemyPos = m_pTiledMap->getEnemyBirthPos();

	//进场先加一次
	this->addEnemy(0);
	//每隔3秒添加敌人
	this->schedule(CC_CALLBACK_1(CEnemyMgr::addEnemy,this),3.0f,"addEnemy");	
}

void CEnemyMgr::update(float delta)
{
	_eventDispatcher->dispatchCustomEvent("enemyCount", &m_nEnemyCount);

}

void CEnemyMgr::addEnemy(float)
{
	
	int nPos = rand() % 2;

	if (m_nEnemyCount > 0)
	{
		if (m_nCurCount < 4)
		{
			switch (nPos)
			{
			case 0:
			{
				CEnemy* pEnemy = CEnemy::createWithData(vecEnemyPos[0]);
				if ( pEnemy )
				{
					this->addChild(pEnemy);
				}
				break;
			}
			case 1:
			{
				CEnemy* pEnemy = CEnemy::createWithData(vecEnemyPos[1]);
				if (pEnemy)
				{
					this->addChild(pEnemy);
				}
				break;
			}
			default:
				break;
			}
		}
	}
}


//敌人移动动画
void CEnemyMgr::loadAnimation()
{
	Rect rc(0, 0, 16, 16);
	for (int i = 0; i < 4; i++)
	{
		string strLev = to_string(i);
		for (int j = 0; j < 4; j++)
		{
			string strDir = to_string(j);
			SpriteFrame* enemy_1 = SpriteFrame::create("images/enemy_tank/normal_tank/" + to_string(i + 1) + "-" + to_string(j + 1) + "-1.png", rc);
			SpriteFrame* enemy_2 = SpriteFrame::create("images/enemy_tank/normal_tank/" + to_string(i + 1) + "-" + to_string(j + 1) + "-2.png", rc);
			enemy_1->getTexture()->setAliasTexParameters();
			enemy_2->getTexture()->setAliasTexParameters();
			Animation* pAniamtion = Animation::createWithSpriteFrames({ enemy_1,enemy_2 }, 0.05f);

			pCache->addSpriteFrame(enemy_1, "enemy_"+strDir +"_"+ strLev);

			VecAnimates[j].pushBack(Animate::create(pAniamtion));
		}
	}
}


cocos2d::Vector<Animate*>* CEnemyMgr::getAnimates()
{
	return VecAnimates;
}

void CEnemyMgr::setCurCount(int nSub)
{
	m_nCurCount += nSub;
}

void CEnemyMgr::setEnemyCount(int nSub)
{
	m_nEnemyCount += nSub;
}
