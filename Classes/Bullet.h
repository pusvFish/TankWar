#ifndef _Bullet_H_
#define _Bullet_H_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class CBullet : public Node
{
public:
	CBullet();
	~CBullet();
	static CBullet* createWithData(int nDir, int nLevel);
	bool initWithData(int nDir, int nLevel);
	virtual void update(float delta);
	CC_SYNTHESIZE_READONLY(Sprite*, m_pImg, Img);
	CC_SYNTHESIZE(Vec2, vecVector, Vector);
public:
	Rect getWorldBoundingBox();
	void setBulletPosition(const Vec2& pos);
private:
	bool m_bIsMove;
	int m_nDir;
	int m_nLevel;
	bool m_bBoomDelay;
	bool m_bBlastDelay;
	bool m_bBulletColli;
	int m_nDamage;
private:
	void autoMove();//�����Զ��ƶ�
	void addObject();//���sprite
	void overMap();//ײǽ��������Ч
	void overBlock();//ײש��������Ч
	void overTank();//ײ̹��
	void overBullet();//ײ�ӵ�
	void removeRromVec();
	bool isPlayerBullet();
	Vec2 getColliPos(const Vec2& vec);
	string getColliStr(const Vec2& pos);
	void boomAndRemove();
	void blastAndRemove();
};




#endif // !

