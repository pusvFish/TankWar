#ifndef _TiledMapMgr_H_
#define _TiledMapMgr_H_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class CTiledMapMgr : public Node
{
public:
	CTiledMapMgr();
	~CTiledMapMgr();
	virtual  bool init();
	CREATE_FUNC(CTiledMapMgr);
	CC_SYNTHESIZE(vector<Vec2>, m_vecPlayerBirthpos, PlayerBirthPos);
	CC_SYNTHESIZE(vector<Vec2>, m_vecEnemyBirthpos, EnemyBirthPos);
	CC_SYNTHESIZE_READONLY(TMXTiledMap*, m_pTiledMap, TiledMap);
public:
	string checkColliLayer(const Vec2& tile);
	Vec2 getTileByPos( Vec2& worldPos);
	Vec2 getPosByTile(const Vec2& tile);
	void removeWithPos(Vec2 WorldPos);//ÏÞ¶¨ÆÕÍ¨×©¿é
	void eagleDead();
private:
	int m_nLevelID;
	TMXLayer* m_pBlock;
	TMXLayer* m_pIronBlock;
	TMXLayer* m_pWater;
	TMXLayer* m_pGlass;
	TMXLayer* m_pEagle;
private:
	void initBirthPos();
	void initMap();
	void initLayers();
	
};
#endif


