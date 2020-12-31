#ifndef _PlayerMgr_H_
#define _PlayerMgr_H_
#include "cocos2d.h"
#include "Player.h"
USING_NS_CC;
using namespace std;
class CPlayerMgr : public Node
{
public:
	CPlayerMgr();
	~CPlayerMgr();

	virtual bool init();
	CREATE_FUNC(CPlayerMgr);
	virtual void onEnter();
	virtual void update(float delta);
public:
	Vector<Animate*>* getAnimates();
private:
	void addPlayer();
	void addSpriteCache();
	void loadAnimation();
	void rebirth(EventCustom* pEvent);
private:
	SpriteFrameCache* pCache;
	Vector<Animate*> VecAnimates[4];
	CPlayer* m_pPlayer;
	int m_nLives;
};
#endif
