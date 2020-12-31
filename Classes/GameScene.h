#ifndef _GameScene_H_
#define _GameScene_H_
#include "cocos2d.h"
#include "TiledMapMgr.h"
#include "PlayerMgr.h"
#include "EnemyMgr.h"
#include "IconMgr.h"
#include "BulletMgr.h"
USING_NS_CC;
using namespace std;

/*
1、player.cpp与enemy.cpp未抽出整合父类
2、子弹碰子弹消失有bug，未开启
3、道具未实现
4、结束加分界面未完成
5、子弹发射频率未控制
6、未绘制第2张地图，换关功能未测试，（重开可以，预计没问题）(记得加载loadScene.h)
*/



class CGameScene : public Scene
{
public:
	~CGameScene();
	static CGameScene* getInstance();
	virtual bool init();
	CC_SYNTHESIZE_READONLY(CTiledMapMgr*, m_pTiledMapMgr, TiledMapMgr);
	CC_SYNTHESIZE_READONLY(CEnemyMgr*, m_pEnemyMgr, EnemyMgr);
	CC_SYNTHESIZE_READONLY(CPlayerMgr*, m_pPlayerMgr, PlayerMgr);
	CC_SYNTHESIZE_READONLY(CIconMgr*, m_pIconMgr, IconMgr);
	CC_SYNTHESIZE_READONLY(CBulletMgr*, m_pBulletMgr, BulletMgr);
public:
	void over();
	void destroyAndNext();
private:
	Vec2 origin;
	Size visibleSize;
	static CGameScene* m_pInstance;
	map<EventKeyboard::KeyCode, Dir> table;
	bool m_bDeadOnce;
private:
	CREATE_FUNC(CGameScene);
	CGameScene();
	void addKeyBoardEvent();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent);
	void setDirtion();
};



#endif // !_GameMenu_H_

