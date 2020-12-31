#include "Player.h"
#include "GameScene.h"
#include "TiledMapMgr.h"
CPlayer::CPlayer()
	:m_nDir(E_DIR_UP),
	m_nLevel(0),
	m_bIsMove(false),
	m_bIsShoot(false),
	m_nMusicID(0),
	m_pImg(nullptr),
	m_pRing(nullptr),
	m_nBlood(0)
{
}

CPlayer::~CPlayer()
{
}


bool CPlayer::init()
{
	if (!Node::init())
	{
		return false;
	}
	pCache = SpriteFrameCache::getInstance();
	birthPos =  CGameScene::getInstance()->getTiledMapMgr()->getPlayerBirthPos()[0];

	this->birthStart();

	this->scheduleUpdate();
	return true;
}

void CPlayer::update(float delta)
{
	if (m_bIsMove)
	{
		this->autoMove();
	}	
	if (m_pRing && m_pImg)
	{
		m_pRing->setPosition(m_pImg->getPosition());
	}
}

void CPlayer::upgrade()
{
	if ( m_nLevel >= 3 )
	{
		return;
	}
	m_nLevel++;
	m_nBlood = (m_nLevel + 1);
	string strLevel = to_string(m_nLevel);
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("player1_1_" + strLevel);
	m_pImg->setDisplayFrame(pFrame);
}
bool CPlayer::disBlood(int nDamage)
{
	m_nBlood -= nDamage;
	if (m_nBlood <= 0)
	{
		m_bIsMove = false;
		int nA = 1;
		_eventDispatcher->dispatchCustomEvent("rebirth", &nA);//死亡重生
		this->removeAllChildrenWithCleanup(true);
		this->removeFromParent();
		return true;
	}
	return false;
}
//闪烁星星动画
void CPlayer::birthStart()
{
	Sprite* pStar = Sprite::createWithSpriteFrameName("star_0");
	this->addChild(pStar);
	pStar->setPosition(birthPos);
	Animation* pAniStar = Animation::create();
	for (int i = 0; i < 4; i++)
	{
		string strNum = to_string(i);
		SpriteFrame* pFrame = pCache->getSpriteFrameByName("star_" + strNum);
		pAniStar->addSpriteFrame(pFrame);
	}
	pAniStar->setDelayPerUnit(0.2f);
	Animate* pAni = Animate::create(pAniStar);
	pStar->runAction(RepeatForever::create(pAni));
	pStar->runAction(
		CallFunc::create( //移除星星
			[=]()
			{
				scheduleOnce(
					[=](float) {
						pStar->stopAllActions();
						pStar->removeFromParent();
						this->addObject();
						this->beInvisble();
						m_bIsShoot = true;
					},
					1.0f,
						"removeStar");
			}
	));
}
//坦克无敌护身
void CPlayer::beInvisble()
{
	m_pRing = Sprite::createWithSpriteFrameName("ring_0");
	this->addChild(m_pRing);
	m_pRing->setPosition(birthPos);
	Animation* pAniRing = Animation::create();
	for (int i = 0; i < 2; i++)
	{
		string strNum = to_string(i);
		SpriteFrame* pFrame = pCache->getSpriteFrameByName("ring_" + strNum);
		pAniRing->addSpriteFrame(pFrame);
	}
	pAniRing->setDelayPerUnit(0.1f);
	Animate* pAni = Animate::create(pAniRing);
	m_pRing->runAction(RepeatForever::create(pAni));
	m_pRing->runAction(
		CallFunc::create( //移除ring
			[=]()
			{
				scheduleOnce(
					[=](float) {
						m_pRing->stopAllActions();
						m_pRing->removeFromParent();
						m_pRing = nullptr; },
					3.0f,
							"removeRing");
			}
	));
}

void CPlayer::addObject()
{
	m_nBlood = (m_nLevel + 1);
	string strLevel = to_string(m_nLevel);
	m_pImg = Sprite::createWithSpriteFrameName("player1_1_" + strLevel);
	this->addChild(m_pImg);
	this->setPosition(birthPos);
}

void CPlayer::startMove()
{
	m_bIsMove = true;
	//m_nMusicID = cocos2d::experimental::AudioEngine::play2d("music/player_move.mp3");
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("player1_" + to_string(m_nDir) + "_" + to_string(m_nLevel));
	m_pImg->setDisplayFrame(pFrame);
}

void CPlayer::stopMove()
{
	m_bIsMove = false;
	//cocos2d::experimental::AudioEngine::stop(m_nMusicID);
}

void CPlayer::startAnimate()
{
	Animate* pAnimate = CGameScene::getInstance()->getPlayerMgr()->getAnimates()[m_nDir].at(m_nLevel);
	m_pImg->runAction(RepeatForever::create(pAnimate));
}



void CPlayer::stopAnimate()
{
	m_pImg->stopAllActions();
}

//按帧数移动
void CPlayer::autoMove()
{
	Vec2 pos = this->getPosition();
	float fOffset = 0.5f + m_nLevel * 0.2f;

	switch (m_nDir)
	{
	case E_DIR_LEFT:
		this->setPositionX(pos.x - fOffset);
		break;
	case E_DIR_UP:
		this->setPositionY(pos.y + fOffset);
		break;
	case E_DIR_RIGHT:
		this->setPositionX(pos.x + fOffset);
		break;
	case E_DIR_DOWN:
		this->setPositionY(pos.y - fOffset);
		break;
	default:
		break;
	}

	if (this)
	{
		if (this->overMap() || this->overBlock())
		{
			vecStrColli.clear();
			this->setPosition(pos);
		}
	}	
}

void CPlayer::fire()
{
	if (m_bIsShoot)
	{
		CBulletMgr* pBulletMgr = CGameScene::getInstance()->getBulletMgr();
		CBullet* pBullet = CBullet::createWithData(m_nDir,m_nLevel);
		pBullet->setBulletPosition(this->getPosition());

		pBulletMgr->addBullet(pBullet);
		pBulletMgr->getPlayerBullets().pushBack(pBullet);
	}
}

bool CPlayer::overMap()
{
	Vec2 pos = this->getPosition();
	return pos.x < 32 ||
		pos.x > 223.5 ||
		pos.y > 208 ||
		pos.y < 16.49;
}

bool CPlayer::overBlock()
{
	vector<string> strColli = this->checkColli(m_nDir);
	for (string strBac : strColli)
	{
		if ("block" == strBac)
		{
			m_bIsMove = false;
			return true;
		}
		else if ("ironBlock" == strBac)
		{
			m_bIsMove = false;
			return true;
		}
		else if ("water" == strBac)
		{
			m_bIsMove = false;
			return true;
		}
	}

	return false;
}

Rect CPlayer::getWorldBoundingBox()
{
	Rect recImg = m_pImg->getBoundingBox();
	Rect rc;
	rc.size = recImg.size;
	rc.origin = this->convertToWorldSpaceAR(recImg.origin);
	return rc;
}

//碰撞点碰到的图层
vector<string> CPlayer::checkColli(int nDir)
{	
	//8个点
	int arrUp[6] = {-1,1,0,1,1,1};
	int arrRight[6] = {1,1,1,0,1,-1};
	int arrDown[6] = {-1,-1,0,-1,1,-1};
	int arrLeft[6] = {-1,1,-1,0,1,-1};

	int j = 0;
	for (int i = 0; i < 3; i++ )
	{
		switch (m_nDir)
		{
		case E_DIR_LEFT:
		{
			this->setColliStr(Vec2(arrLeft[j],arrLeft[j+1]));
			j += 2;
			break;
		}	
		case E_DIR_UP:
		{
			this->setColliStr(Vec2(arrUp[j], arrUp[1 + j]));
			j += 2;
			break;
		}
		case E_DIR_RIGHT:
		{
			this->setColliStr(Vec2(arrRight[j], arrRight[1 + j]));
			j += 2;
			break;
		}
		case E_DIR_DOWN:
		{
			this->setColliStr(Vec2(arrDown[j], arrDown[1 + j]));
			j += 2;
			break;
		}
		default:
			break;
		}
	}
	j = 0;
	
	return vecStrColli;
}
//根据方向的碰撞坐标
Vec2 CPlayer::getColliPos(const Vec2& vec)
{
	Vec2 pos = this->getPosition();
	Size size = m_pImg->getContentSize();
	float fX = pos.x + (vec.x * size.width / 8 * 3 );
	float fY = pos.y + (vec.y * size.height / 8 * 3 );
	return Vec2(fX,fY);
}

//设置移动方向要碰的3个点
void CPlayer::setColliStr(const Vec2& pos)
{
	CTiledMapMgr* pTiledMapMgr = CGameScene::getInstance()->getTiledMapMgr();
	Vec2 colliPos = this->getColliPos(pos);
	Vec2 tile = pTiledMapMgr->getTileByPos(colliPos);
	string strColli = pTiledMapMgr->checkColliLayer(tile);
	vecStrColli.push_back(strColli);
}

void CPlayer::overTank()
{
}
