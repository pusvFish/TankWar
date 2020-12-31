#ifndef _IconMgr_H_
#define _IconMgr_H_
#include "cocos2d.h"
#include "Icon.h"
USING_NS_CC;
using namespace std;
class CIconMgr : public Node
{
public:
	CIconMgr();
	~CIconMgr();
	virtual bool init();
	CREATE_FUNC(CIconMgr);
	CC_SYNTHESIZE(Sprite*, m_pLive, Live);
	CC_SYNTHESIZE(int, m_nLives, Lives);
private:
	Size size;
	Vec2 origin;
	SpriteFrameCache* pCache;
	int m_nIconCount;
	int m_nCurIconCount;
	Texture2D* m_pTexture;
private:
	Vector<CIcon*> VecIcons;
	void addIcons();
	void deleteIcon(EventCustom* pEvent);
	void addPlayerFlag();//Íæ¼ÒÉúÃüÊý
	void changeLive(EventCustom* pEvent);
};
#endif
