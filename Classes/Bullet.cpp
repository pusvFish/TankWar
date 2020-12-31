#include "Bullet.h"
#include "GameScene.h"
#include "AudioEngine.h"
CBullet::CBullet()
	:m_bIsMove(false),
	m_nDir(0),
	m_nLevel(0),
	m_bBoomDelay(false),
	m_bBlastDelay(false),
	m_nDamage(0),
	m_bBulletColli(false)
{
	vecVector = Vec2(0, 0);
}

CBullet::~CBullet()
{
}

CBullet* CBullet::createWithData(int nDir, int nLevel)
{
	CBullet* pBullet = new CBullet();
	if (pBullet && pBullet->initWithData(nDir, nLevel))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	pBullet = nullptr;
	return nullptr;
}

bool CBullet::initWithData(int nDir, int nLevel)
{
	if (!Node::init())
	{
		return false;
	}
	//确认方向  
	m_nDir = nDir;
	m_nDamage = (nLevel + 1);
	//图片
	this->addObject();
	this->scheduleUpdate();
	return true;
}

void CBullet::update(float delta)
{
	if (m_bIsMove)
	{
		this->autoMove();
	}
}



void CBullet::autoMove()
{
	Vec2 pos = this->getPosition();
	float fSpeed = 0.8f + (0.2f * m_nLevel);

	switch (m_nDir)
	{
	case E_DIR_LEFT:
	{
		this->setPositionX(pos.x - fSpeed);
		break;
	}
	case E_DIR_UP:
	{
		this->setPositionY(pos.y + fSpeed);
		break;
	}
	case E_DIR_RIGHT:
	{
		this->setPositionX(pos.x + fSpeed);
		break;
	}
	case E_DIR_DOWN:
	{
		this->setPositionY(pos.y - fSpeed);
		break;
	}
	default:
		break;
	}

	this->overMap();

	if (this)
	{
		this->overBlock();
	}

	if (this)
	{
		this->overTank();
	}

	if (this)
	{
		this->overBullet();
	}
}

void CBullet::addObject()
{
	m_pImg = Sprite::createWithSpriteFrameName("bullet_" + to_string(m_nDir));
	this->addChild(m_pImg);
	m_bIsMove = true;
}
//
void CBullet::overMap()
{
	Vec2 pos = this->getPosition();
	if (pos.x < 26 || pos.x > 230 || pos.y > 214 || pos.y < 10)
	{
		m_bIsMove = false;
		if (!m_bBoomDelay)//开启计时器，避免每帧检测,干一次
		{
			m_bBoomDelay = true;
			if (this->isPlayerBullet())//玩家子弹发声
			{
				cocos2d::experimental::AudioEngine::play2d("music/bin.mp3");
			}

			this->boomAndRemove();
		}

	}

}
//
void CBullet::overBlock()
{
	CTiledMapMgr* pTiledMapMgr = CGameScene::getInstance()->getTiledMapMgr();
	string strBlock = this->getColliStr(vecVector);

	if ("block" == strBlock)
	{
		pTiledMapMgr->removeWithPos(this->getPosition());
		m_bIsMove = false;
		this->boomAndRemove();

	}
	else if ("ironBlock" == strBlock)
	{
		if (this->isPlayerBullet())
		{
			cocos2d::experimental::AudioEngine::play2d("music/bin.mp3");
		}
		m_bIsMove = false;
		this->boomAndRemove();
	}
	else if ("eagle" == strBlock)
	{
		m_bIsMove = false;
		this->boomAndRemove();
		CGameScene::getInstance()->getTiledMapMgr()->eagleDead();
		//跳转
		CGameScene::getInstance()->over();
	}
	else if ("water" == strBlock)
	{
		this->setVisible(false);
	}
	else
	{
		this->setVisible(true);
	}
}
//
void CBullet::overTank()
{
	if (!m_bBlastDelay)
	{
		if (this->isPlayerBullet())
		{
			Vector<Node*> VecEnemy = CGameScene::getInstance()->getEnemyMgr()->getChildren();
			if (VecEnemy.size() > 0)
			{
				for (Node* pNode : VecEnemy)
				{
					CEnemy* pEnemy = static_cast<CEnemy*>(pNode);
					if (pEnemy && pEnemy->getImg())
					{
						if (this->getWorldBoundingBox().intersectsRect(pEnemy->getWorldBoundingBox()))
						{
							m_bIsMove = false;
							m_bBlastDelay = true;
							if (pEnemy->disBlood(m_nDamage))
							{
								this->blastAndRemove();
							}
							else
							{
								this->removeRromVec();
								this->removeFromParent();
							}
						}
					}
				}
			}
		}
		else
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(CGameScene::getInstance()->getPlayerMgr()->getChildByName("player"));
			if (pPlayer && pPlayer->getImg())
			{
				if (this->getWorldBoundingBox().intersectsRect(pPlayer->getWorldBoundingBox()))
				{
					m_bIsMove = false;
					m_bBlastDelay = true;
					if (pPlayer->disBlood(m_nDamage))
					{
						this->blastAndRemove();
					}
					else
					{
						this->removeRromVec();
						this->removeFromParent();
					}
				}
			}
		}
	}
}
void CBullet::overBullet()
{
	if (!m_bBulletColli)
	{
		/*if (this->isPlayerBullet())
		{
			Vector<CBullet*> VecEnemyBullet = CGameScene::getInstance()->getBulletMgr()->getEnemyBullets();
			Vector<CBullet*> VecPlayerBullet = CGameScene::getInstance()->getBulletMgr()->getPlayerBullets();

			if (VecEnemyBullet.size() > 0 && VecPlayerBullet.size() > 0 )
			{
				for (Vector<CBullet*>::iterator iter1 = VecPlayerBullet.begin(); iter1 != VecPlayerBullet.end(); )
				{
					for (Vector<CBullet*>::iterator iter2 = VecEnemyBullet.begin(); iter2 != VecEnemyBullet.end();)
					{
						if ((*iter1)->getWorldBoundingBox().intersectsRect((*iter2)->getWorldBoundingBox()))
						{
							m_bBulletColli = true;
							CCLOG("aaaa");
							VecPlayerBullet.erase(iter1);
							iter2 = VecEnemyBullet.erase(iter2);
							(*iter2)->removeFromParent();
							(*iter1)->removeFromParent();
							break;
						}
						else
						{
							++iter2;
						}
					}
					++iter1;
				}

			}
		}*/
		//if (this->isPlayerBullet())
		//{
		//	Vector<CBullet*> VecEnemyBullet = CGameScene::getInstance()->getBulletMgr()->getEnemyBullets();
		//	if (VecEnemyBullet.size() > 0)
		//	{
		//		for (Vector<CBullet*>::iterator iter = VecEnemyBullet.begin(); iter != VecEnemyBullet.end();)
		//		{
		//			if (this->getWorldBoundingBox().intersectsRect((*iter)->getWorldBoundingBox()))
		//			{
		//				m_bBulletColli = true;
		//				CCLOG("bbbbb");
		//				(*iter)->removeFromParentAndCleanup(true);
		//				//(*iter)->removeFromParent();
		//				iter = VecEnemyBullet.erase(iter);
		//				this->removeRromVec();
		//				this->removeFromParent();
		//			}
		//			else
		//			{
		//				++iter;
		//			}
		//		}
		//	}
		//}
	}

}
void CBullet::removeRromVec()
{
	//清理子弹
	Vector<CBullet*> VecPlayerBullet = CGameScene::getInstance()->getBulletMgr()->getPlayerBullets();
	if (VecPlayerBullet.size() > 0)
	{
		for (Vector<CBullet*>::iterator iter = VecPlayerBullet.begin(); iter != VecPlayerBullet.end();)
		{
			if (this == *iter)
			{
				iter = VecPlayerBullet.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	//清理子弹
	Vector<CBullet*> VecEnemyBullet = CGameScene::getInstance()->getBulletMgr()->getEnemyBullets();
	if (VecEnemyBullet.size() > 0)
	{
		for (Vector<CBullet*>::iterator iter = VecEnemyBullet.begin(); iter != VecEnemyBullet.end();)
		{
			if (this == *iter)
			{
				iter = VecEnemyBullet.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}
bool CBullet::isPlayerBullet()
{
	Vector<CBullet*> VecPlayerBullet = CGameScene::getInstance()->getBulletMgr()->getPlayerBullets();
	if (VecPlayerBullet.size() > 0)
	{
		for (Vector<CBullet*>::iterator iter = VecPlayerBullet.begin(); iter != VecPlayerBullet.end();)
		{
			if (this == *iter)
			{
				return true;
			}
			else
			{
				++iter;
			}
		}
	}
	return false;
}
Vec2 CBullet::getColliPos(const Vec2& vec)
{
	//判断不精准
	Vec2 pos = this->getPosition();
	//Size size = m_pImg->getContentSize();
	//float fX = pos.x + (vec.x * size.width / 2);
	//float fY = pos.y + (vec.y * size.height / 2);
	return pos;
}
//
string CBullet::getColliStr(const Vec2& pos)
{
	CTiledMapMgr* pTiledMapMgr = CGameScene::getInstance()->getTiledMapMgr();
	Vec2 colliPos = this->getColliPos(pos);
	Vec2 tile = pTiledMapMgr->getTileByPos(colliPos);
	string strColli = pTiledMapMgr->checkColliLayer(tile);
	return strColli;
}
void CBullet::boomAndRemove()
{
	Animate* pAnimate = CGameScene::getInstance()->getBulletMgr()->getAnimates().at(0);
	m_pImg->runAction(RepeatForever::create(pAnimate));
	this->scheduleOnce([this](float) {
		m_pImg->stopAllActions();
		this->removeRromVec();
		this->removeFromParent();
		},
		0.15f,
			"removeSelf_1");
}

void CBullet::blastAndRemove()
{
	Animate* pAnimate = CGameScene::getInstance()->getBulletMgr()->getAnimates().at(1);
	m_pImg->runAction(RepeatForever::create(pAnimate));

	this->scheduleOnce([this](float) {
		m_pImg->stopAllActions();
		this->removeRromVec();
		this->removeFromParent();
		},
		0.5f,
			"removeSelf_2");
}

//
Rect CBullet::getWorldBoundingBox()
{
	Rect rcImg = m_pImg->getBoundingBox();
	Rect rc;
	rc.size = rcImg.size;
	rc.origin = this->convertToWorldSpaceAR(rcImg.origin);
	return rc;
}
//
void CBullet::setBulletPosition(const Vec2& pos)
{
	switch (m_nDir)
	{
	case E_DIR_LEFT:
	{
		vecVector = Vec2(-1, 0);
		this->setPosition(pos.x - 7, pos.y);
		break;
	}
	case E_DIR_UP:
	{
		vecVector = Vec2(0, 1);
		this->setPosition(pos.x, pos.y + 7);
		break;
	}
	case E_DIR_RIGHT:
	{
		vecVector = Vec2(1, 0);
		this->setPosition(pos.x + 7, pos.y);
		break;
	}
	case E_DIR_DOWN:
	{
		vecVector = Vec2(0, -1);
		this->setPosition(pos.x, pos.y - 7);
		break;
	}
	default:
		break;
	}
}
