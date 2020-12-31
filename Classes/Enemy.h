#ifndef _Enemy_H_
#define _Enemy_H_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;


class CEnemy : public Node
{
public:
	CEnemy();
	~CEnemy();
	static CEnemy* createWithData(const Vec2& pos);
	bool  initWithData(const Vec2& pos);
	virtual void update(float delta);
	CC_SYNTHESIZE_READONLY(Sprite*, m_pImg, Img);//初始化指针
	CC_SYNTHESIZE(int, m_nLevel, Level);
	CC_SYNTHESIZE(int, m_nDir, Dir);
	CC_SYNTHESIZE(bool, m_bIsMove, IsMove);
	CC_SYNTHESIZE(bool, m_bIsShoot, IsShoot);
	CC_SYNTHESIZE(Vec2, vecVector, Vector);
public:
	Rect getWorldBoundingBox();
	bool disBlood(int nDamage);
private:
	Vec2 vecPos;
	SpriteFrameCache* pCache;
	vector<string> vecStrColli;
	float m_fFireDeley;
	int m_nBlood;
private:
	void fire();
	void autoFire();
	bool isPlaceOn(const Vec2& pos);
	void birthStar();
	void addObject();
	void autoMove();
	void startMove();
	void stopMove();
	void startAnimate();
	void stopAnimate();
	bool overMap();
	bool overBlock();
	bool overTank();
	vector<string> checkColli(int nDir);
	Vec2 getColliPos(const Vec2& vec);
	void setColliStr(const Vec2& pos);
	void changeDir();
	void autoChange();//碰撞后自动反向
};


#endif // !

