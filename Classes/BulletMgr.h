#ifndef _BulletMgr_H_
#define _BulletMgr_H_
#include "cocos2d.h"
#include "AudioEngine.h"
#include "Bullet.h"
USING_NS_CC;
using namespace std;

class CBulletMgr : public Node
{
public:
	CBulletMgr();
	~CBulletMgr();
	virtual bool init();
	CREATE_FUNC(CBulletMgr);
public:
	void addBullet(CBullet* pBullet);
	Vector<CBullet*>& getPlayerBullets();
	Vector<CBullet*>& getEnemyBullets();
	Vector<Animate*> getAnimates();
private:
	void addSpriteCache();
	void loadAnimation();
private:
	SpriteFrameCache* pCache;
	Vector<CBullet*> VecPlayerBullet;
	Vector<CBullet*> VecEnemyBullet;
	Vector<Animate*> VecAnimates;
};



#endif 


