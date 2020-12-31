#include "BulletMgr.h"

CBulletMgr::CBulletMgr()
{
}

CBulletMgr::~CBulletMgr()
{
}

bool CBulletMgr::init()
{
	if (!Node::init())
	{
		return false;
	}
	//
	pCache = SpriteFrameCache::getInstance();

	this->addSpriteCache();
	this->loadAnimation();
	return true;
}

void CBulletMgr::addBullet(CBullet* pBullet)
{
	if (pBullet)
	{
		this->addChild(pBullet);
	}
}
//Íæ¼Ò×Óµ¯
Vector<CBullet*>& CBulletMgr::getPlayerBullets()
{
	return VecPlayerBullet;
}
//µÐÈË×Óµ¯
Vector<CBullet*>& CBulletMgr::getEnemyBullets()
{
	return VecEnemyBullet;
}
//±¬Õ¨¶¯»­
Vector<Animate*> CBulletMgr::getAnimates()
{
	return VecAnimates;
}

void CBulletMgr::addSpriteCache()
{
	char szName[32] = {};
	//×Óµ¯
	for ( int i = 0; i < 4; i++ )
	{
		sprintf_s(szName, "images/bullet/bullet-%d.png", i);
		SpriteFrame* pFrame = Sprite::create(szName)->getSpriteFrame();
		pFrame->getTexture()->setAliasTexParameters();
		pCache->addSpriteFrame(pFrame, "bullet_" + to_string(i));
	}

	//×²Ç½±¬Õ¨
	for (int i = 0; i < 3; i++)
	{
		sprintf_s(szName, "images/bullet/bumb%d.png", i);
		SpriteFrame* pFrame = Sprite::create(szName)->getSpriteFrame();
		pFrame->getTexture()->setAliasTexParameters();
		pCache->addSpriteFrame(pFrame, "bumb_" + to_string(i));
	}
}

void CBulletMgr::loadAnimation()
{
	char szName[32] = {};

	//×Óµ¯±¬Õ¨¶¯»­
	Animation* pBulletAni = Animation::create();
	for ( int i = 0; i < 3; i++ )
	{
		sprintf_s(szName, "bumb_%d", i);
		SpriteFrame* pFrame = pCache->getSpriteFrameByName(szName);
		pBulletAni->addSpriteFrame(pFrame);
	}
	pBulletAni->setDelayPerUnit(0.05f);
	VecAnimates.pushBack(Animate::create(pBulletAni));

	//Ì¹¿Ë±¬Õ¨
	Animation* pTankAni = Animation::create();
	for (int i = 0; i < 5; i++)
	{
		sprintf_s(szName, "blast_%d", i);
		SpriteFrame* pFrame = pCache->getSpriteFrameByName(szName);
		pTankAni->addSpriteFrame(pFrame);
	}
	pTankAni->setDelayPerUnit(0.1f);
	VecAnimates.pushBack(Animate::create(pTankAni));
}
