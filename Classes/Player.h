#ifndef _Player_H_
#define _Player_H_
#include "cocos2d.h"
#include "AudioEngine.h"
USING_NS_CC;
using namespace std;
enum  Dir
{
	E_DIR_LEFT,
	E_DIR_UP,
	E_DIR_RIGHT,
	E_DIR_DOWN
};
class CPlayer : public Node
{
public:
	CPlayer();
	~CPlayer();
	virtual bool init();
	virtual void update(float delta);
	CREATE_FUNC(CPlayer);
	CC_SYNTHESIZE_READONLY(Sprite*, m_pImg, Img);//定义变量，初始化
	CC_SYNTHESIZE(int, m_nDir, Dir);
	CC_SYNTHESIZE(int, m_nLevel, Level);
	CC_SYNTHESIZE(bool, m_bIsMove, IsMove);
	CC_SYNTHESIZE(bool, m_bIsShoot, IsShoot);
public:
	void fire();
	void upgrade();
	bool disBlood(int nDamage);
	void birthStart();
	void addObject();
	void startMove();
	void stopMove();
	void startAnimate();
	void beInvisble();
	void stopAnimate();
	void autoMove();
	bool overMap();
	bool overBlock();
	void overTank();
	Rect getWorldBoundingBox();
	vector<string> checkColli(int nDir);
	Vec2 getColliPos(const Vec2& vec);
	void setColliStr(const Vec2& pos);
private:
	SpriteFrameCache* pCache;
	int m_nMusicID;
	vector<string> vecStrColli;
	Vec2 birthPos;
	Sprite* m_pRing;
	int m_nBlood;
};


#endif 

