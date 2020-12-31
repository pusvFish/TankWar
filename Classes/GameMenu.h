#ifndef _GameMenu_H_
#define _GameMenu_H_
#include "cocos2d.h"
USING_NS_CC;

class CGameMenu : public Scene
{
public:
	CGameMenu();
	~CGameMenu();
	virtual bool init();
	CREATE_FUNC(CGameMenu);
private:
	Vec2 origin;
	Size visibleSize;
	Sprite* m_pBg;
	Animation* m_pAnimationTank;
	Sprite* m_pTankSelector;
	int m_nState;
private:
	void initBackGround();

	void fallDownEvent();
	void changeEvent();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent);
	bool onTouchBegan(Touch* pTouch, Event* pEvent);

	void addAnimation();
	void initTankSelector();
};


#endif 

