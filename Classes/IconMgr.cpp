#include "IconMgr.h"
#include "DynamicDt.h"
#include "NumberUtil.h"
CIconMgr::CIconMgr():
	m_nIconCount(0),
	m_nCurIconCount(0),
	m_nLives(0),
	m_pLive(nullptr),
	m_pTexture(nullptr)
{
}

CIconMgr::~CIconMgr()
{
}

bool CIconMgr::init()
{
	if (!Node::init() )
	{
		return false;
	}
	//
	size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	pCache = SpriteFrameCache::getInstance();

	//获取图标总数
	m_nIconCount = CDynamicDt::getInstance()->getEnemyCount();
	m_nLives = CDynamicDt::getInstance()->getLives();

	//准备图标精灵帧
	SpriteFrame* pIcon = SpriteFrame::create("images/enemytank-ico.png", Rect(0, 0, 7, 7));
	pCache->addSpriteFrame(pIcon,"icon");
	SpriteFrame* pPlayerIcon = SpriteFrame::create("images/playertank-ico.png", Rect(0,0,7,8));
	pCache->addSpriteFrame(pPlayerIcon, "playerIcon");
	SpriteFrame* p1_p = SpriteFrame::create("images/1Pnew.png", Rect(0, 0, 14, 7));
	pCache->addSpriteFrame(p1_p, "1p");
	//添加所有图标 
	this->addIcons();

	//玩家生命量
	this->addPlayerFlag();

	//
	_eventDispatcher->addCustomEventListener("enemyCount",CC_CALLBACK_1(CIconMgr::deleteIcon,this));
	_eventDispatcher->addCustomEventListener("lives", CC_CALLBACK_1(CIconMgr::changeLive, this));
	return true;
}


void CIconMgr::addIcons()
{
	float fHeight = size.height / 2 + 20;
	float fWidth = size.width - 18;

	for ( int i = 0; i < m_nIconCount; i++ )
	{
		CIcon* pIcon = CIcon::create();
		this->addChild(pIcon);
		VecIcons.pushBack(pIcon);
		Size sizeIcon = pIcon->getImg()->getContentSize();
		pIcon->setPosition(
			Vec2
			(
				fWidth + (i % 2) * (sizeIcon.width + 1),
				fHeight + (i / 2)*(sizeIcon.height +1)
			)
		);
	}
}

void CIconMgr::deleteIcon(EventCustom* pEvent)
{
	int* pCount = static_cast<int*>(pEvent->getUserData());
	if ( *pCount < m_nIconCount )
	{
		int nSub = m_nIconCount - (*pCount);
		for (int i = 0; i < nSub; i++)
		{
			
			this->removeChild(*(VecIcons.begin()));
			VecIcons.erase(VecIcons.begin());
		}
		m_nIconCount = *pCount;
	}
}

void CIconMgr::addPlayerFlag()
{
	Sprite* p1_p = Sprite::createWithSpriteFrameName("1p");
	this->addChild(p1_p);
	p1_p->getTexture()->setAliasTexParameters();
	p1_p->setPosition(Vec2(size.width - 14, 40));

	Sprite* pPlayerIcon = Sprite::createWithSpriteFrameName("playerIcon");
	this->addChild(pPlayerIcon);
	pPlayerIcon->getTexture()->setAliasTexParameters();
	pPlayerIcon->setPosition(Vec2(size.width - 19, 30));

	m_pLive = CNumberUtil::getYellowNumber(m_nLives);
	this->addChild(m_pLive);
	m_pLive->setPosition(Vec2(size.width - 10, 30));
}


void CIconMgr::changeLive(EventCustom* pEvent)
{
	int* nLive = static_cast<int*>(pEvent->getUserData());
	if ( *nLive <  m_nLives )
	{
		Sprite* pLive = CNumberUtil::getYellowNumber(*nLive);
		SpriteFrame* pFrame = pLive->getSpriteFrame();
		m_pLive->setDisplayFrame(pFrame);
		m_nLives = *nLive;
	}
}
