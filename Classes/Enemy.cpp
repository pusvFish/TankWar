#include "Enemy.h"
#include "GameScene.h"
CEnemy::CEnemy()
	:m_nLevel(0),
	m_nDir(E_DIR_DOWN),
	m_bIsMove(false),
	m_bIsShoot(false),
	m_pImg(nullptr),
	m_fFireDeley(0.0f),
	m_nBlood(0)
{
	vecVector = Vec2(0, 0);
}

CEnemy::~CEnemy()
{
}

CEnemy* CEnemy::createWithData(const Vec2& pos)
{
	CEnemy* pEnemy = new CEnemy();
	if (pEnemy && pEnemy->initWithData(pos))
	{
		pEnemy->autorelease();
		return pEnemy;
	}
	CC_SAFE_DELETE(pEnemy);
	pEnemy = nullptr;
	return nullptr;
}

bool CEnemy::initWithData(const Vec2& pos)
{
	if (!Node::init())
	{
		return false;
	}
	pCache = SpriteFrameCache::getInstance();
	//保存出生坐标
	vecPos = pos;
	//检测生成位置是否被敌人或者玩家占据，有则不生成，返回false,判断敌人和玩家包围盒是否包裹坐标
	if (this->isPlaceOn(pos))
	{
		return false;
	}
	//管理者总数减1，画面坦克计数加1
	CGameScene::getInstance()->getEnemyMgr()->setEnemyCount(-1);
	CGameScene::getInstance()->getEnemyMgr()->setCurCount(1);
	//播放星星
	this->birthStar();
	//随机level，方向向下,产生敌人，开启移动开关, 开启开火开关
	this->scheduleUpdate();
	//随机方向移动
	this->schedule(
		[=](float)
		{
			m_nDir = rand() % 4;
			this->autoChange();
		},
		2.0f,
			"changeDir");

	//随机射子弹
	return true;
}

void CEnemy::update(float delta)
{
	if (m_bIsMove)
	{
		this->autoMove();
	}
}

void CEnemy::startMove()
{
	m_bIsMove = true; //移动
	SpriteFrame* pFrame = pCache->getSpriteFrameByName("enemy_" + to_string(m_nDir) + "_" + to_string(m_nLevel));
	m_pImg->setDisplayFrame(pFrame);
}

void CEnemy::stopMove()
{
	m_bIsMove = false;
}

void CEnemy::startAnimate()
{
	Animate* pAnimate = CGameScene::getInstance()->getEnemyMgr()->getAnimates()[m_nDir].at(m_nLevel);
	m_pImg->runAction(RepeatForever::create(pAnimate));
}

void CEnemy::stopAnimate()
{
	m_pImg->stopAllActions();
}

//地图碰撞
bool CEnemy::overMap()
{
	Vec2 pos = this->getPosition();
	return pos.x < 32 ||
		pos.x > 223.5 ||
		pos.y > 208 ||
		pos.y < 16.49;
}
//砖块碰撞
bool CEnemy::overBlock()
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
bool CEnemy::overTank()
{
	Vector<Node*> vecNode = CGameScene::getInstance()->getEnemyMgr()->getChildren();
	if (vecNode.size() > 0)
	{
		for (Node* pNode : vecNode)
		{
			CEnemy* pEnemy = static_cast<CEnemy*>(pNode);
			if (pEnemy && pEnemy->getImg() && this && this->getImg())

				if (pEnemy != this && this->getWorldBoundingBox().intersectsRect(pEnemy->getWorldBoundingBox()))
				{
					return true;
				}

		}
	}

	return false;
}
//根据方向取3个点的碰撞信息
vector<string> CEnemy::checkColli(int nDir)
{
	//8个点
	int arrUp[6] = { -1,1,0,1,1,1 };
	int arrRight[6] = { 1,1,1,0,1,-1 };
	int arrDown[6] = { -1,-1,0,-1,1,-1 };
	int arrLeft[6] = { -1,1,-1,0,1,-1 };

	int j = 0;
	for (int i = 0; i < 3; i++)
	{
		switch (m_nDir)
		{
		case E_DIR_LEFT:
		{
			this->setColliStr(Vec2(arrLeft[j], arrLeft[j + 1]));
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

Vec2 CEnemy::getColliPos(const Vec2& vec)
{

	Vec2 pos = this->getPosition();
	Size size = m_pImg->getContentSize();
	float fX = pos.x + (vec.x * size.width / 8 * 3);
	float fY = pos.y + (vec.y * size.height / 8 * 3);
	return Vec2(fX, fY);
}

void CEnemy::setColliStr(const Vec2& pos)
{
	CTiledMapMgr* pTiledMapMgr = CGameScene::getInstance()->getTiledMapMgr();
	Vec2 colliPos = this->getColliPos(pos);
	Vec2 tile = pTiledMapMgr->getTileByPos(colliPos);
	string strColli = pTiledMapMgr->checkColliLayer(tile);
	vecStrColli.push_back(strColli);
}

void CEnemy::changeDir()
{
	switch (m_nDir)
	{
	case E_DIR_LEFT:
	{
		m_nDir = E_DIR_RIGHT;
		this->autoChange();
		break;
	}
	case E_DIR_UP:
	{
		m_nDir = E_DIR_DOWN;
		this->autoChange();
		break;
	}
	case E_DIR_RIGHT:
	{
		m_nDir = E_DIR_LEFT;
		this->autoChange();
		break;
	}
	case E_DIR_DOWN:
	{
		m_nDir = E_DIR_UP;
		this->autoChange();
		break;
	}
	default:
		break;
	}
}

void CEnemy::autoChange()
{
	this->stopMove();
	this->stopAnimate();
	this->startMove();
	this->startAnimate();
}

//出生星星
void CEnemy::birthStar()
{
	Sprite* pStar = Sprite::createWithSpriteFrameName("star_0");
	this->addChild(pStar);
	pStar->setPosition(vecPos);
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
						this->startMove();
						this->startAnimate();
						m_bIsShoot = true; //开火
						this->autoFire();

					},
					1.0f,
						"removeStar");
			}
	));
}

//添加随机等级敌人
void CEnemy::addObject()
{
	m_nLevel = rand() % 4;
	m_nBlood = (m_nLevel + 1);
	m_pImg = Sprite::createWithSpriteFrameName("enemy_" + to_string(m_nDir) + "_" + to_string(m_nLevel));
	this->addChild(m_pImg);
	this->setPosition(vecPos);
}

void CEnemy::autoMove()
{
	Vec2 pos = this->getPosition();
	float fOffset = 0.4f + m_nLevel * 0.1f;

	switch (m_nDir)
	{
	case E_DIR_LEFT:
		vecVector = Vec2(-1, 0);
		this->setPositionX(pos.x - fOffset);
		break;
	case E_DIR_UP:
		vecVector = Vec2(0, 1);
		this->setPositionY(pos.y + fOffset);
		break;
	case E_DIR_RIGHT:
		vecVector = Vec2(1, 0);
		this->setPositionX(pos.x + fOffset);
		break;
	case E_DIR_DOWN:
		vecVector = Vec2(0, -1);
		this->setPositionY(pos.y - fOffset);
		break;
	default:
		break;
	}

	if (this->overMap() || this->overBlock() || this->overTank())
	{
		vecStrColli.clear();
		this->setPosition(pos);
		//立刻转向
		//this->changeDir();
	}
}

void CEnemy::fire()
{
	if (m_bIsShoot)
	{
		CBulletMgr* pBulletMgr = CGameScene::getInstance()->getBulletMgr();
		CBullet* pBullet = CBullet::createWithData(m_nDir,m_nLevel);
		pBullet->setBulletPosition(this->getPosition());

		pBulletMgr->addBullet(pBullet);
		pBulletMgr->getEnemyBullets().pushBack(pBullet);
	}
}

void CEnemy::autoFire()
{
	if ( 0 == m_nLevel)
	{
		m_fFireDeley = 3.0f;
	}
	else
	{
		m_fFireDeley = 3.0f / (m_nLevel + 1);
	}
	this->schedule([this](float) {this->fire(); }, m_fFireDeley, "fireEnemy");
}

//判断出生点被占
bool CEnemy::isPlaceOn(const Vec2& pos)
{
	//是否玩家占据敌人出生点
	CPlayer* pPlayer = static_cast<CPlayer*>(CGameScene::getInstance()->getPlayerMgr()->getChildByName("player"));
	if (pPlayer && pPlayer->getImg())
	{
		if (pPlayer->getWorldBoundingBox().containsPoint(pos))
		{
			return true;
		}
	}

	//敌人占据出生点
	Vector<Node*> vecEnemy = CGameScene::getInstance()->getEnemyMgr()->getChildren();
	if (vecEnemy.size() > 0)
	{
		for (Node* pNode : vecEnemy)
		{
			CEnemy* pEnemy = static_cast<CEnemy*>(pNode);
			if (pEnemy && pEnemy->getImg())
			{
				if (pEnemy->getWorldBoundingBox().containsPoint(pos))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//包围盒
Rect CEnemy::getWorldBoundingBox()
{
	Rect rcImg = m_pImg->getBoundingBox();
	Rect rc;
	rc.size = rcImg.size;
	rc.origin = this->convertToWorldSpaceAR(rcImg.origin);
	return rc;
}

bool CEnemy::disBlood(int nDamage)
{
	m_nBlood -= nDamage;
	if (m_nBlood <= 0)
	{
		m_bIsMove = false;
		CGameScene::getInstance()->getEnemyMgr()->setCurCount(-1);
		this->removeAllChildrenWithCleanup(true);
		this->removeFromParent();
		return true;
	}
	return false;
}
