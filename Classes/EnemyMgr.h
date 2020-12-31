#ifndef _EnemyMgr_H_
#define _EnemyMgr_H_
#include "cocos2d.h"
#include "TiledMapMgr.h"
#include "Enemy.h"
USING_NS_CC;
using namespace std;

class CEnemyMgr : public Node
{
public:
	CEnemyMgr();
	~CEnemyMgr();

	virtual bool init();
	virtual void onEnter();
	virtual void update(float delta);
	CREATE_FUNC(CEnemyMgr);
	CC_SYNTHESIZE_READONLY(int, m_nCurCount, CurCount);
	CC_SYNTHESIZE_READONLY(int, m_nEnemyCount, EnemyCount);
	
public:
	Vector<Animate*>* getAnimates();
	void setCurCount(int nSub);
	void setEnemyCount(int nSub);
private:
	void loadAnimation();
	void addEnemy(float);
private:
	SpriteFrameCache* pCache;
	Vector<Animate*> VecAnimates[4];
	vector<Vec2> vecEnemyPos;
	CTiledMapMgr* m_pTiledMap;


};
#endif

