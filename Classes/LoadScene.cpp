#include "LoadScene.h"
#include "GameScene.h"
#include "NumberUtil.h"
#include "DynamicDt.h"
CLoadScene::CLoadScene()
	:m_nLevelID(0)
{
}

CLoadScene::~CLoadScene()
{
}

bool CLoadScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//界面数据
	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	LayerColor* pLayerColor = LayerColor::create(Color4B(100, 100, 100, 255));
	this->addChild(pLayerColor);

	this->loadAnimation();
	//
	m_pState = Sprite::create("images/stage.png");
	m_pState->getTexture()->setAliasTexParameters();
	m_pState->setPosition(Vec2(visibleSize.width / 2 - 10, visibleSize.height / 2));
	this->addChild(m_pState);
	//
	m_nLevelID = CDynamicDt::getInstance()->getLevelID();
	//关卡数字
	this->levelID();
	return true;
}

void CLoadScene::levelID()
{
	Vec2 pos = m_pState->getPosition();
	//十位
	Sprite* pTenSprite = CNumberUtil::getBalckNumber(m_nLevelID / 10);
	pTenSprite->getTexture()->setAliasTexParameters();
	pTenSprite->setPosition(Vec2(pos.x + m_pState->getContentSize().width, pos.y));
	this->addChild(pTenSprite);
	if ( m_nLevelID / 10 == 0 )
	{
		pTenSprite->setVisible(false);
	}

	//个位
	Sprite* pSinSprite = CNumberUtil::getBalckNumber(m_nLevelID % 10);
	pSinSprite->getTexture()->setAliasTexParameters();
	pSinSprite->setPosition(Vec2(pos.x + m_pState->getContentSize().width + 10, pos.y));
	this->addChild(pSinSprite);
}

void CLoadScene::loadAnimation()
{
	int nWidth = visibleSize.width;
	int nHeight = visibleSize.height / 2;

	LayerColor* pUpColor = LayerColor::create(Color4B(0, 0, 0, 255));
	LayerColor* pDownColor = LayerColor::create(Color4B(0, 0, 0, 255));

	pUpColor->setContentSize(Size(nWidth, nHeight));
	pDownColor->setContentSize(Size(nWidth, nHeight));

	pUpColor->setPosition(Vec2(origin.x, origin.y + visibleSize.height));
	pDownColor->setPosition(Vec2(origin.x, -visibleSize.height / 2));

	this->addChild(pUpColor);
	this->addChild(pDownColor);

	pUpColor->runAction(Sequence::create(
		MoveBy::create(0.7,Vec2(0,-nHeight)),
		CallFunc::create([=]() {this->removeChild(pUpColor); }),
		nullptr));

	pDownColor->runAction(Sequence::create(
		MoveBy::create(0.7, Vec2(0, nHeight)),
		CallFunc::create([=]() {this->removeChild(pDownColor); }),
		nullptr));


	//进入游戏场景
	this->scheduleOnce(
		[](float) { Director::getInstance()->replaceScene(TransitionFade::create(0.8f,CGameScene::getInstance()));},
		1.2f,
		"gameScene");
}


